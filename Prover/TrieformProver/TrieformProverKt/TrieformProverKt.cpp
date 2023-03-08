#include "TrieformProverKt.h"

map<vector<int>, shared_ptr<TrieformProverKt>> TrieformProverKt::all_trieforms;

shared_ptr<Trieform>
TrieformFactory::makeTrieKt(const shared_ptr<Formula> &formula,
                           shared_ptr<Trieform> trieParent) {
  shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverKt());
  trie->initialise(formula, trieParent);
  return trie;
}
shared_ptr<Trieform>
TrieformFactory::makeTrieKt(const shared_ptr<Formula> &formula,
                           const vector<int> &newModality,
                           shared_ptr<Trieform> trieParent) {
  shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverKt());
  trie->initialise(formula, newModality, trieParent);
  return trie;
}
shared_ptr<Trieform>
TrieformFactory::makeTrieKt(const vector<int> &newModality,
                           shared_ptr<Trieform> trieParent) {
  shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverKt());
  trie->initialise(newModality, trieParent);
  return trie;
}

TrieformProverKt::TrieformProverKt() {
    stringModalContexts=true;
}
TrieformProverKt::~TrieformProverKt() {}

shared_ptr<Trieform>
TrieformProverKt::create(const shared_ptr<Formula> &formula) {
  return TrieformFactory::makeTrieKt(formula, shared_from_this());
}
shared_ptr<Trieform> TrieformProverKt::create(const shared_ptr<Formula> &formula,
                                             const vector<int> &newModality) {
  return TrieformFactory::makeTrieKt(formula, newModality, shared_from_this());
}
shared_ptr<Trieform> TrieformProverKt::create(const vector<int> &newModality) {
  shared_ptr<Trieform> test = shared_from_this();
  return TrieformFactory::makeTrieKt(newModality, shared_from_this());
}

shared_ptr<Bitset>
TrieformProverKt::convertAssumptionsToBitset(literal_set literals) {
  shared_ptr<Bitset> bitset =
      shared_ptr<Bitset>(new Bitset(2 * assumptionsSize));
  for (Literal literal : literals) {
    bitset->set(2 * idMap[literal.getName()] + literal.getPolarity());
  }
  return bitset;
}

void TrieformProverKt::updateSolutionMemo(const shared_ptr<Bitset> &assumptions,
                                         Solution solution) {
  if (solution.satisfiable) {
    localMemo.insertSat(assumptions);
  } else {
    localMemo.insertUnsat(assumptions, solution.conflict);
  }
}

void TrieformProverKt::getStats() {

  for (auto modalSubtrie : subtrieMap) {
    auto subtrie = dynamic_cast<TrieformProverKt*>(modalSubtrie.second.get());
    subtrie->getStats();
    numRelations = max(abs(modalSubtrie.first),subtrie->numRelations);
  }
}

shared_ptr<TrieformProverKt> TrieformProverKt::convertToGrid(vector<int>& modalContext) {
    if (all_trieforms.find(modalContext) == all_trieforms.end()) {
        all_trieforms[modalContext] = shared_ptr<TrieformProverKt>(new TrieformProverKt);
    }     

    shared_ptr<TrieformProverKt> gridTrieform = all_trieforms[modalContext];

    gridTrieform->clauses.extendClauses(getClauses());

    for (auto modalSubtrie: subtrieMap) {

        modalContext[abs(modalSubtrie.first)] += (modalSubtrie.first > 0) ? 1 : -1;
        shared_ptr<TrieformProverKt> gridTrieformSuccessor = dynamic_cast<TrieformProverKt*>(modalSubtrie.second.get())->convertToGrid(modalContext); 
        modalContext[abs(modalSubtrie.first)] -= (modalSubtrie.first > 0) ? 1 : -1;
        
        gridTrieform->subtrieMap[modalSubtrie.first] = gridTrieformSuccessor;
        gridTrieformSuccessor->subtrieMap[-modalSubtrie.first] = gridTrieform;
        
    }
    
    return gridTrieform;
}

void TrieformProverKt::preprocess() {
  doResiduation();
}

shared_ptr<TrieformProverKt> TrieformProverKt::createGridTrie() {
  getStats();
  vector<int> modal_context (numRelations+1);
  auto replacement = convertToGrid(modal_context);
  replacement->numRelations = numRelations;
  return replacement;
  //replacement.buildConnections();
}

bool TrieformProverKt::isSatisfiable() {
    literal_set assumps;
    assumps.insert(Literal("$root", true));
    return prove(assumps).satisfiable;
}

Solution TrieformProverKt::prove(literal_set assumptions) {
  // Check solution memo
  shared_ptr<Bitset> assumptionsBitset =
      convertAssumptionsToBitset(assumptions);
  LocalSolutionMemoResult memoResult = localMemo.getFromMemo(assumptionsBitset);

  if (memoResult.inSatMemo) {
    return memoResult.result;
  }

  if (isInHistory(history, assumptionsBitset)) {
    return {true, literal_set()};
  }

  // Solve locally
  Solution solution = prover->solve(assumptions);

  if (!solution.satisfiable) {
    updateSolutionMemo(assumptionsBitset, solution);
    return solution;
  }
    
  prover->calculateTriggeredDiamondsClauses();
  modal_literal_map triggeredDiamonds = prover->getTriggeredDiamondClauses();

  // If there are no fired diamonds, it is satisfiable
  if (triggeredDiamonds.size() == 0) {
    updateSolutionMemo(assumptionsBitset, solution);
    return solution;
  }

  prover->calculateTriggeredBoxClauses();
  modal_literal_map triggeredBoxes = prover->getTriggeredBoxClauses();
  
  for (auto modalitySubtrie : subtrieMap) {
    // Handle each modality
    if (triggeredDiamonds[modalitySubtrie.first].size() == 0) {
      // If there are no triggered diamonds of a certain modality we can skip it
      continue;
    }
    if (isSubsetOf(triggeredDiamonds[modalitySubtrie.first],
                   triggeredBoxes[modalitySubtrie.first])) {
      // The fired diamonds are a subset of the boxes - we thus can create one
      // world.
      history.push_back(assumptionsBitset);
      Solution childSolution =
          modalitySubtrie.second.get()->prove(triggeredBoxes[modalitySubtrie.first]);
      history.pop_back();

      // If the one world solution is satisfiable, then we're all good
      if (childSolution.satisfiable) {
        continue;
      }
      // Otherwise, as the diamonds are a subset of the boxes, the left
      // implications of the problem box clauses cannot be true with any diamond
      // clause of this modality
      vector<literal_set> badImplications = prover->getNotProblemBoxClauses(
          modalitySubtrie.first, childSolution.conflict);
      badImplications.push_back(
          prover->getNotAllDiamondLeft(modalitySubtrie.first));
      // Add ~leftDiamond=>\/~leftProbemBox
      for (literal_set learnClause : generateClauses(badImplications)) {
        prover->addClause(learnClause);
      }
      // Find new result
      return prove(assumptions);
    } else {
      // The fired diamonds are not a subset of the fired boxes, we need to
      // create one world for each diamond clause
      diamond_queue diamondPriority =
          prover->getPrioritisedTriggeredDiamonds(modalitySubtrie.first);

      while (!diamondPriority.empty()) {
        // Create a world for each diamond
        Literal diamond = diamondPriority.top().literal;
        diamondPriority.pop();

        literal_set childAssumptions =
            literal_set(triggeredBoxes[modalitySubtrie.first]);
        childAssumptions.insert(diamond);

      history.push_back(assumptionsBitset);
        // Run the solver for the next level
      Solution childSolution =
          modalitySubtrie.second.get()->prove(childAssumptions);
      history.pop_back();

        // If it is satisfiable create the next world
        if (childSolution.satisfiable) {
          continue;
        }

        // Otherwise there must have been a conflict
        vector<literal_set> badImplications = prover->getNotProblemBoxClauses(
            modalitySubtrie.first, childSolution.conflict);

        if (childSolution.conflict.find(diamond) !=
            childSolution.conflict.end()) {
          // The diamond clause, either on its own or together with box clauses,
          // caused a conflict. We must add diamond implies OR NOT problem
          // box clauses.
          prover->updateLastFail(diamond);
          badImplications.push_back(
              prover->getNotDiamondLeft(modalitySubtrie.first, diamond));

          for (literal_set learnClause : generateClauses(badImplications)) {
            prover->addClause(learnClause);
          }

          // Find new result
          return prove(assumptions);
        } else {
          // Only the box clauses caused a conflict, so we must add each diamond
          // clause implies OR NOT problem box lefts
          badImplications.push_back(
              prover->getNotAllDiamondLeft(modalitySubtrie.first));
          // Add ~leftDiamond=>\/~leftProbemBox
          for (literal_set learnClause : generateClauses(badImplications)) {
            prover->addClause(learnClause);
          }
          // Find new result
          return prove(assumptions);
        }
      }
    }
  }
  // If we reached here the solution is satisfiable under all modalities
  updateSolutionMemo(assumptionsBitset, solution);
  return solution;
}

void TrieformProverKt::doResiduation() {
    map<vector<int>, modal_clause_set> newClauses;
    for (auto modTrie : all_trieforms) {
        auto modalContext = modTrie.first;
        for (ModalClause modalClause : modTrie.second->getClauses().getBoxClauses()) {
            modalContext[abs(modalClause.modality)] += (modalClause.modality>0) ? 1 : -1;
            newClauses[modalContext].insert({-modalClause.modality,
                              modalClause.right->negate(),
                              modalClause.left->negate()});
            modalContext[abs(modalClause.modality)] -= (modalClause.modality>0) ? 1 : -1;

        }

    }
    for (auto modTrie : all_trieforms) {
        modTrie.second->clauses.extendBoxClauses(newClauses[modTrie.first]);
    }
}

void TrieformProverKt::buildConnections() {
    for (auto modTrie : all_trieforms) {
        auto modalContext = modTrie.first;
        for (ModalClause modalClause : modTrie.second->getClauses().getDiamondClauses()) {
            modalContext[abs(modalClause.modality)] += (modalClause.modality>0) ? 1 : -1;
            
            modTrie.second->subtrieMap[modalClause.modality] = all_trieforms[modalContext]; 
            
            modalContext[abs(modalClause.modality)] -= (modalClause.modality>0) ? 1 : -1;

        }

    }
}
string TrieformProverKt::toString() {
    if (numRelations == -1) {
        return Trieform::toString();
    }
    string trieString = "";
    for (auto modTrie : all_trieforms) {
        string modalContext = "";
        for (unsigned int i = 1; i < modTrie.first.size(); ++i) {
            modalContext += "[" + to_string(i) + "](" 
                + to_string(modTrie.first[i])+")";
        }

        vector<string> clauseComponents = modTrie.second->clauses.toStringComponents();

        for (string component : clauseComponents) {
            trieString += modalContext + " " + component + '\n';
        }
    }
    return trieString; 
}

bool TrieformProverKt::isInHistory(vector<shared_ptr<Bitset>> history,
                                   shared_ptr<Bitset> bitset) {
  for (shared_ptr<Bitset> assump : history) {
    if (assump->contains(*bitset)) {
      return true;
    }
  }
  return false;
}

void TrieformProverKt::removeTrueAndFalse() {
    for (auto modTrie : all_trieforms) {
        modTrie.second->clauses.removeTrueAndFalse();
    }
}

void TrieformProverKt::prepareSAT(name_set extra_unused) {
    for (auto modTrie : all_trieforms) {
        vector<string> clauseComponents = modTrie.second->clauses.toStringComponents();

        vector<int> modalContext = modTrie.first;
        name_set extra;

        if (all_of(modalContext.begin(), modalContext.end(), [](int i) { return i==0; })) {
            extra.insert("$root");
        }

        for (int jump : {-1, 1}) for (int relation = 1; relation <= numRelations; ++relation) {
            modalContext[relation] += jump;
            
            if (all_trieforms.find(modalContext) == all_trieforms.end()) continue;

            auto ancestorTrie = all_trieforms[modalContext];
            for (ModalClause clause : ancestorTrie->getClauses().getBoxClauses()) {
                if (clause.modality == -jump) {
                    extra.insert(ancestorTrie->getProver()->getPrimitiveName(clause.right));
                }
            }
            for (ModalClause clause : ancestorTrie->getClauses().getDiamondClauses()) {
                if (clause.modality == -jump) {
                    extra.insert(ancestorTrie->getProver()->getPrimitiveName(clause.right));
                }
            }
            
            modalContext[relation] -= jump;

        }
          for (string name : extra) {
                modTrie.second->idMap[name] = modTrie.second->assumptionsSize++;
          }
          modTrie.second->prover->prepareSAT(modTrie.second->clauses, extra);
    }
}
