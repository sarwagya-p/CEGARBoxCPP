#include "TrieformProverS5.h"
#include <fstream>
bool TrieformProverS5::useGroupDia = true;

unsigned int TrieformProverS5::assumptionsSize = 0;
unordered_map<string, unsigned int> TrieformProverS5::idMap =
    unordered_map<string, unsigned int>();

shared_ptr<Trieform>
TrieformFactory::makeTrieS5(const shared_ptr<Formula> &formula,
                            shared_ptr<Trieform> trieParent, bool oneDia) {
  shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverS5());
  trie->initialise(formula, trieParent);
  TrieformProverS5::useGroupDia = !oneDia;
  return trie;
}
shared_ptr<Trieform>
TrieformFactory::makeTrieS5(const shared_ptr<Formula> &formula,
                            const vector<int> &newModality,
                            shared_ptr<Trieform> trieParent, bool oneDia) {
  shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverS5());
  trie->initialise(formula, newModality, trieParent);
  TrieformProverS5::useGroupDia = !oneDia;
  return trie;
}
shared_ptr<Trieform>
TrieformFactory::makeTrieS5(const vector<int> &newModality,
                            shared_ptr<Trieform> trieParent, bool oneDia) {
  shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverS5());
  trie->initialise(newModality, trieParent);
  TrieformProverS5::useGroupDia = !oneDia;
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
    // prover->updateLastFail(diamond);
    badImplications.push_back(
        prover->getNotDiamondLeft(modality, diamond));

    for (literal_set learnClause : generateClauses(badImplications)) {
      prover->addClause(learnClause);
    }
}

Solution TrieformProverS5::prove_one_dia(literal_set assumptions = literal_set()) {
  Solution soln = prover->solve(assumptions);

  while(soln.satisfiable){
    bool allModalitiesSatisfied = true;

    prover->calculateTriggeredDiamondsClauses();
    modal_literal_map triggeredDiamonds = prover->getTriggeredDiamondClauses();

    if (triggeredDiamonds.size() == 0){
      return {true, literal_set()};
    }

    prover->calculateTriggeredBoxClauses();
    modal_literal_map triggeredBoxes = prover->getTriggeredBoxClauses();
  
    auto modalityDiamonds = *triggeredDiamonds.begin();
    if (modalityDiamonds.second.size() == 0) {
      // If there are no triggered diamonds of a certain modality we can skip
      // it
      continue;
    }
    // Note in the cases diamonds are a subset of boxes then we don't need to
    // create any worlds (reflexivity satisfies this)
    diamond_set diamondPriority =
        prover->getPrioritisedTriggeredDiamondsSet(
          modalityDiamonds.first, 
          triggeredBoxes[modalityDiamonds.first], 
          modalityDiamonds.second);

    shared_ptr<Trieform> subtrie = getSubtrie(modalityDiamonds.first);

    while (!diamondPriority.empty()) {
      // Create a world for each diamond if necessary
      Literal diamond = (*diamondPriority.rbegin()).literal;
      diamondPriority.erase(prev(diamondPriority.end()));

      if (prover->modelSatisfiesAssump(diamond)) {
        continue;
      }

      literal_set childAssumptions = triggeredBoxes[modalityDiamonds.first];
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
          modalityDiamonds.first, childSoln.conflict);

      // The diamond clause, either on its own or together with box clauses,
        // caused a conflict. We must add diamond implies OR NOT problem box
        // clauses.
        prover->updateLastFail(diamond);
        badImplications.push_back(
            prover->getNotDiamondLeft(modalityDiamonds.first, diamond));

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
      return soln;
    }
  }
  // At this point, all modalities are satisfied
  return soln;
}

Solution TrieformProverS5::prove_group_dia(literal_set assumptions = literal_set()) {
  Solution soln = prover->solve(assumptions);

  // If this is a leaf, exit
  if (getTrieMap().empty()) return soln;

  // If unsat, return
  while (soln.satisfiable)
  {
    // cout << "Root sat, trying to prove subtries" << endl;
    // Else, prove for the subtrie
    prover->calculateTriggeredDiamondsClauses();
    modal_literal_map triggeredDiamonds = prover->getTriggeredDiamondClauses();

    if (triggeredDiamonds.size() == 0){
      return {true, literal_set()};
    }

    prover->calculateTriggeredBoxClauses();

    modal_literal_map allModalitiesTriggeredBoxes = prover->getTriggeredBoxClauses();
    literal_set triggeredBoxes;
    if (allModalitiesTriggeredBoxes.empty())
      triggeredBoxes = {};
    else
      triggeredBoxes = allModalitiesTriggeredBoxes.begin()->second;

    // Since currectly only dealing with one modality, this is only set of triggered dias
    auto modalityDiamonds = triggeredDiamonds.begin();
    if (modalityDiamonds->second.size() == 0) {
      return {true, literal_set()};
    }
    // Try making a world with all the diamonds
    // cout << "Trying for all dias" << endl;
    shared_ptr<Trieform> subtrie = getSubtrie(modalityDiamonds->first);

    // Keep track of dias that are not boxes, to know when to stop trying
    literal_set remaining_dias = modalityDiamonds->second;

    for (auto assump: triggeredBoxes){
      if (remaining_dias.find(assump) != remaining_dias.end()){
        remaining_dias.erase(assump);
      }
    }

    literal_set all_dias_assumptions = triggeredBoxes;
    all_dias_assumptions.insert(remaining_dias.begin(), remaining_dias.end());
    
    // cout << "Trying for all dias" << endl;
    
    // While we do not get a sat, keep removing trouble diamonds and retrying
    Solution subtrie_soln = {false, literal_set()};
    bool conflict_dia_exists = true;
    bool unsat_found = false;

    while (conflict_dia_exists && !subtrie_soln.satisfiable && !remaining_dias.empty()){
      subtrie_soln = subtrie->prove(all_dias_assumptions);
      conflict_dia_exists = false;
      for (Literal assump: subtrie_soln.conflict){
        if (triggeredBoxes.find(assump) != triggeredBoxes.end())
          continue;
        conflict_dia_exists = true;

        // Try assump:
        triggeredBoxes.insert(assump);
        Solution assump_soln = subtrie->prove(triggeredBoxes);
        triggeredBoxes.erase(assump);

        if (assump_soln.satisfiable){
          for (auto it=remaining_dias.begin(); it != remaining_dias.end(); ){
            if (subtrie->getProver()->modelSatisfiesAssump(*it)){
              all_dias_assumptions.erase(*it);
              it = remaining_dias.erase(it);
            }
            else{
              ++it;
            }
          }
        }
        else {
          learnClauseOnFailure(assump_soln, assump, modalityDiamonds->first);
          // cout << "Conflict: ";
          // for (Literal lit: subtrie_soln.conflict){
          //   cout << lit.getName() << " ";
          // }
          // cout << endl << "Assump failed, retrying" << endl;
          // return prove(assumptions);
          unsat_found = true;
          break;
        }
      }
      if (unsat_found)
        break;
    }

    // At this point, either subtrie_soln is satisfiable and we return sat, 
    // or we found a dia for which the current valuation doesn't work and we restart
    // or we found a UC containing only boxes, and we restart
    if (subtrie_soln.satisfiable){
      return soln;
    }

    if (unsat_found){
      soln = prover->solve(assumptions);
      continue;
    }

    vector<literal_set> badImplications = prover->getNotProblemBoxClauses(
        modalityDiamonds->first, subtrie_soln.conflict);

    for (literal_set learnClause : generateClauses(badImplications)) {
      prover->addClause(learnClause);
    }
    // cout << "Box conflict found, retrying" << endl;
    soln = prover->solve(assumptions);
  }

  return soln;
}

Solution TrieformProverS5::prove(literal_set assumptions){
  return TrieformProverS5::useGroupDia ? prove_group_dia(assumptions) : prove_one_dia(assumptions);
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