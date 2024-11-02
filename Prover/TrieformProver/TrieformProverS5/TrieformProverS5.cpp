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

Solution TrieformProverS5::prove(literal_set assumptions = literal_set()) {

  Solution soln = prover->solve(assumptions);

  // If this is a leaf, exit
  if (getTrieMap().empty()) return soln;

  int num_its = 0;
  while(soln.satisfiable){
    num_its++;
    bool allModalitiesSatisfied = true;

    prover->calculateTriggeredDiamondsClauses();
    modal_literal_map triggeredDiamonds = prover->getTriggeredDiamondClauses();

    if (triggeredDiamonds.size() == 0){
      cout << "Num its: " <<  num_its << endl;
      return {true, literal_set()};
    }

    prover->calculateTriggeredBoxClauses();
    literal_set triggeredBoxes = prover->getTriggeredBoxClauses().begin()->second;

    // Since currectly only dealing with one modality, this is only set of triggered dias
    auto modalityDiamonds = triggeredDiamonds.begin();
    if (modalityDiamonds->second.size() == 0) {
      cout << "Num its: " <<  num_its << endl;
      return {true, literal_set()};
    }

    // Try making a world with all the diamonds
    // cout << "Trying for all dias" << endl;
    shared_ptr<Trieform> subtrie = getSubtrie(modalityDiamonds->first);

    literal_set no_conflict_assumptions = triggeredBoxes;
    // Keep track of dias that are not boxes, to know when to stop trying
    literal_set no_conflict_dias = modalityDiamonds->second;

    // cout << "Inserting and deleting" << endl;

    for (auto assump: triggeredBoxes){
      if (no_conflict_dias.find(assump) != no_conflict_dias.end()){
        no_conflict_dias.erase(assump);
      }
    }

    no_conflict_assumptions.insert(no_conflict_dias.begin(), no_conflict_dias.end());

    Solution no_conflict_soln = subtrie->prove(no_conflict_assumptions);

    // While we do not get a sat, keep removing trouble diamonds and retrying
    // cout << "Running while loop" << endl;
    bool updated = true;

    while (updated && !no_conflict_soln.satisfiable && !no_conflict_dias.empty()){
      // cout << "Size of no_conflict_dias: " << no_conflict_dias.size() << endl;
      updated = true;
      for (auto assump: no_conflict_soln.conflict){
        if (triggeredBoxes.find(assump) != triggeredBoxes.end())
          continue;
        updated = false;

        no_conflict_assumptions.erase(assump);
        no_conflict_dias.erase(assump);
      }
      no_conflict_soln = subtrie->prove(no_conflict_assumptions);
    }
    // cout << "Loop over" << endl;
    // Note in the cases diamonds are a subset of boxes then we don't need to
    // create any worlds (reflexivity satisfies this)
    diamond_set diamondPriority =
        prover->getPrioritisedTriggeredDiamondsSet(
          modalityDiamonds->first, 
          triggeredBoxes, 
          modalityDiamonds->second);

    if (no_conflict_soln.satisfiable){
      for (auto it=diamondPriority.begin(); it != diamondPriority.end();){
        if (subtrie->getProver()->modelSatisfiesAssump(it->literal)){
          it = diamondPriority.erase(it);
        }
        else {
          ++it;
        }
      }
    }
    
    while (!diamondPriority.empty()) {
      // Create a world for each diamond if necessary
      Literal diamond = (*diamondPriority.rbegin()).literal;
      diamondPriority.erase(prev(diamondPriority.end()));

      if (prover->modelSatisfiesAssump(diamond)) {
        continue;
      }

      literal_set childAssumptions = triggeredBoxes;
      childAssumptions.insert(diamond);

      Solution childSoln = subtrie->prove(childAssumptions);

      if (childSoln.satisfiable){
        // If satisfiable, check if other diamonds can be satisfied with this model
        for (auto it=diamondPriority.begin(); it != diamondPriority.end();){
          if (subtrie->getProver()->modelSatisfiesAssump(it->literal)){
            it = diamondPriority.erase(it);
          }
          else {
            ++it;
          }
        }
        // Create world for remaining
        continue;
      }

      // If it returns unsat, find conflict
      vector<literal_set> badImplications = prover->getNotProblemBoxClauses(
          modalityDiamonds->first, childSoln.conflict);

      // The diamond clause, either on its own or together with box clauses,
        // caused a conflict. We must add diamond implies OR NOT problem box
        // clauses.
        prover->updateLastFail(diamond);
        badImplications.push_back(
            prover->getNotDiamondLeft(modalityDiamonds->first, diamond));

        for (literal_set learnClause : generateClauses(badImplications)) {
          prover->addClause(learnClause);
        }

        // Find new result
        // return prove(assumptions);
        allModalitiesSatisfied = false;
        soln = prover->solve(assumptions);
        break;
    }

    if (allModalitiesSatisfied){
      cout << "Num its: " <<  num_its << endl;
      return soln;
    }
  }
  cout << "Num its: " <<  num_its << endl;
  // At this point, all modalities are satisfied
  return soln;
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