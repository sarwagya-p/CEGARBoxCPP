#include "TrieformProverS5.h"
#include <fstream>

unsigned int TrieformProverS5::assumptionsSize = 0;
unordered_map<string, unsigned int> TrieformProverS5::idMap =
    unordered_map<string, unsigned int>();

shared_ptr<Trieform>
TrieformFactory::makeTrieS5(const shared_ptr<Formula> &formula,
                            shared_ptr<Trieform> trieParent) {
  shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverS5());
  trie->initialise(formula, trieParent);
  return trie;
}
shared_ptr<Trieform>
TrieformFactory::makeTrieS5(const shared_ptr<Formula> &formula,
                            const vector<int> &newModality,
                            shared_ptr<Trieform> trieParent) {
  shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverS5());
  trie->initialise(formula, newModality, trieParent);
  return trie;
}
shared_ptr<Trieform>
TrieformFactory::makeTrieS5(const vector<int> &newModality,
                            shared_ptr<Trieform> trieParent) {
  shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverS5());
  trie->initialise(newModality, trieParent);
  return trie;
}

TrieformProverS5::TrieformProverS5() {}
TrieformProverS5::~TrieformProverS5() {}

shared_ptr<Bitset>
TrieformProverS5::convertAssumptionsToBitset(literal_set literals) {
  shared_ptr<Bitset> bitset =
      shared_ptr<Bitset>(new Bitset(2 * assumptionsSize));
  for (Literal literal : literals) {
    bitset->set(2 * idMap[literal.getName()] + literal.getPolarity());
  }
  return bitset;
}

void TrieformProverS5::prepareSAT(name_set extra) {
  // Shortcut only do this for level 1 as reflexivity guarantees every possible
  // assumption is here. Renaming could stuff this up
  for (string name : extra) {
    if (idMap.find(name) == idMap.end()) {
      idMap[name] = assumptionsSize++;
    }
  }
  for (ModalClause clause : clauses.getDiamondClauses()) {
    extra.insert(prover->getPrimitiveName(clause.right));
  }
  modal_names_map modalExtras = prover->prepareSAT(clauses, extra);
  for (auto modalSubtrie : subtrieMap) {
    modalSubtrie.second->prepareSAT(modalExtras[modalSubtrie.first]);
  }
}

void TrieformProverS5::learnClauseOnFailure(Solution& soln, Literal diamond, int modality){
  // If it returns unsat, find conflict
  vector<literal_set> badImplications = prover->getNotProblemBoxClauses(
      modality, soln.conflict);

  // The diamond clause, either on its own or together with box clauses,
    // caused a conflict. We must add diamond implies OR NOT problem box
    // clauses.
    prover->updateLastFail(diamond);
    badImplications.push_back(
        prover->getNotDiamondLeft(modality, diamond));

    for (literal_set learnClause : generateClauses(badImplications)) {
      prover->addClause(learnClause);
    }
}

Solution TrieformProverS5::prove(literal_set assumptions = literal_set()) {

  Solution soln = prover->solve(assumptions);

  // If this is a leaf, exit
  if (getTrieMap().empty()) return soln;

  // Else, prove for the subtrie
  prover->calculateTriggeredDiamondsClauses();
  modal_literal_map triggeredDiamonds = prover->getTriggeredDiamondClauses();

  if (triggeredDiamonds.size() == 0){
    return {true, literal_set()};
  }

  prover->calculateTriggeredBoxClauses();
  literal_set triggeredBoxes = prover->getTriggeredBoxClauses().begin()->second;

  // Since currectly only dealing with one modality, this is only set of triggered dias
  auto modalityDiamonds = triggeredDiamonds.begin();
  if (modalityDiamonds->second.size() == 0) {
    return {true, literal_set()};
  }

  // Try making a world with all the diamonds
  // cout << "Trying for all dias" << endl;
  shared_ptr<Trieform> subtrie = getSubtrie(modalityDiamonds->first);

  // Keep track of dias that are not boxes, to know when to stop trying
  literal_set no_conflict_dias = modalityDiamonds->second;

  // cout << "Inserting and deleting" << endl;

  for (auto assump: triggeredBoxes){
    if (no_conflict_dias.find(assump) != no_conflict_dias.end()){
      no_conflict_dias.erase(assump);
    }
  }

  literal_set no_conflict_assumptions = triggeredBoxes;
  no_conflict_assumptions.insert(no_conflict_dias.begin(), no_conflict_dias.end());

  Solution subtrie_soln = subtrie->prove(no_conflict_assumptions);

  // While we do not get a sat, keep removing trouble diamonds and retrying
  // cout << "Running while loop" << endl;
  bool conflict_dia_exists = true;

  while (conflict_dia_exists && !subtrie_soln.satisfiable && !no_conflict_dias.empty()){
    conflict_dia_exists = false;
    for (auto assump: subtrie_soln.conflict){
      if (triggeredBoxes.find(assump) != triggeredBoxes.end())
        continue;
      conflict_dia_exists = true;

      // Try assump:
      triggeredBoxes.insert(assump);
      Solution assump_soln = subtrie->prove(triggeredBoxes);
      triggeredBoxes.erase(assump);

      if (assump_soln.satisfiable){
        for (auto it=no_conflict_dias.begin(); it != no_conflict_dias.end(); ){
          if (subtrie->getProver()->modelSatisfiesAssump(*it)){
            no_conflict_assumptions.erase(*it);
            it = no_conflict_dias.erase(it);
          }
          else{
            ++it;
          }
        }
      }
      else {
        learnClauseOnFailure(assump_soln, assump, modalityDiamonds->first);
        return prove(assumptions);
      }
    }
    subtrie_soln = subtrie->prove(no_conflict_assumptions);
  }

  // At this point, either subtrie_soln is satisfiable, or we found
  // a UC containing only boxes
  if (subtrie_soln.satisfiable){
    return soln;
  }
  vector<literal_set> badImplications = prover->getNotProblemBoxClauses(
      modalityDiamonds->first, subtrie_soln.conflict);

  for (literal_set learnClause : generateClauses(badImplications)) {
    prover->addClause(learnClause);
  }

  return prove(assumptions);
}

void TrieformProverS5::reflexiveHandleBoxClauses(){
  for (pair<int, shared_ptr<Trieform>> subtrie_pair: subtrieMap){
    for (ModalClause box_clause: subtrie_pair.second->clauses.getBoxClauses()){
      clauses.addBoxClause(box_clause);
    }
  }
}

void TrieformProverS5::preprocess(){
  // reflexiveHandleBoxClauses();
}

shared_ptr<Trieform>
TrieformProverS5::create(const shared_ptr<Formula> &formula) {
  return TrieformFactory::makeTrieS5(formula, shared_from_this());
}
shared_ptr<Trieform>
TrieformProverS5::create(const shared_ptr<Formula> &formula,
                         const vector<int> &newModality) {
  return TrieformFactory::makeTrieS5(formula, newModality, shared_from_this());
}
shared_ptr<Trieform> TrieformProverS5::create(const vector<int> &newModality) {
  return TrieformFactory::makeTrieS5(newModality, shared_from_this());
}