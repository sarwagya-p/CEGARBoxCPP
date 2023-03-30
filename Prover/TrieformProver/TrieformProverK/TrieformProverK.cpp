#include "TrieformProverK.h"

shared_ptr<Trieform>
TrieformFactory::makeTrieK(const shared_ptr<Formula> &formula,
        shared_ptr<Trieform> trieParent) {
    shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverK());
    trie->initialise(formula, trieParent);
    return trie;
}
shared_ptr<Trieform>
TrieformFactory::makeTrieK(const shared_ptr<Formula> &formula,
        const vector<int> &newModality,
        shared_ptr<Trieform> trieParent) {
    shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverK());
    trie->initialise(formula, newModality, trieParent);
    return trie;
}
shared_ptr<Trieform>
TrieformFactory::makeTrieK(const vector<int> &newModality,
        shared_ptr<Trieform> trieParent) {
    shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverK());
    trie->initialise(newModality, trieParent);
    return trie;
}

TrieformProverK::TrieformProverK() {}
TrieformProverK::~TrieformProverK() {}

shared_ptr<Trieform>
TrieformProverK::create(const shared_ptr<Formula> &formula) {
    return TrieformFactory::makeTrieK(formula, shared_from_this());
}
shared_ptr<Trieform> TrieformProverK::create(const shared_ptr<Formula> &formula,
        const vector<int> &newModality) {
    return TrieformFactory::makeTrieK(formula, newModality, shared_from_this());
}
shared_ptr<Trieform> TrieformProverK::create(const vector<int> &newModality) {
    shared_ptr<Trieform> test = shared_from_this();
    return TrieformFactory::makeTrieK(newModality, shared_from_this());
}

shared_ptr<Bitset>
TrieformProverK::convertAssumptionsToBitset(literal_set literals) {
    shared_ptr<Bitset> bitset =
        shared_ptr<Bitset>(new Bitset(2 * assumptionsSize));
    for (Literal literal : literals) {
        bitset->set(2 * idMap[literal.getName()] + literal.getPolarity());
    }
    return bitset;
}

void TrieformProverK::updateSolutionMemo(const shared_ptr<Bitset> &assumptions,
        Solution solution) {
    if (solution.satisfiable) {
        localMemo.insertSat(assumptions);
    } else {
        localMemo.insertUnsat(assumptions, solution.conflict);
    }
}

void TrieformProverK::preprocess() {}

void TrieformProverK::prepareSAT(name_set extra) {
    for (string name : extra) {
        idMap[name] = assumptionsSize++;
    }
    modal_names_map modalExtras = prover->prepareSAT(clauses, extra);
    for (auto modalSubtrie : subtrieMap) {
        modalSubtrie.second->prepareSAT(modalExtras[modalSubtrie.first]);
    }
}

shared_ptr<Bitset>
TrieformProverK::fleshedOutAssumptionBitset(literal_set model) {
    shared_ptr<Bitset> bitset =
        shared_ptr<Bitset>(new Bitset(2 * assumptionsSize)); 
    for (auto x : idMap) {
        Literal lit = Literal{x.first, 0};
        bitset->set(2 * x.second + (model.find(lit) == model.end()));
    }
    return bitset;
}

Solution TrieformProverK::prove(literal_set assumptions) {
    cout << "Proving: ";
    for (auto x : assumptions) cout << x.toString() << " "; cout << endl;
    // Check solution memo
    shared_ptr<Bitset> assumptionsBitset =
        convertAssumptionsToBitset(assumptions);
    LocalSolutionMemoResult memoResult = localMemo.getFromMemo(assumptionsBitset);

    if (memoResult.inSatMemo) {
        return memoResult.result;
    }

    // Solve locally
    Solution solution = prover->solve(assumptions);

    if (!solution.satisfiable) {
        // prover->reduce_conflict(solution.conflict);
        updateSolutionMemo(assumptionsBitset, solution);
        return solution;
    }

    literal_set currentModel = prover->getModel();
    assumptionsBitset = fleshedOutAssumptionBitset(currentModel);

    prover->calculateTriggeredDiamondsClauses();
    modal_literal_map triggeredDiamonds = prover->getTriggeredDiamondClauses();
    prover->calculateTriggeredBoxClauses();
    modal_literal_map triggeredBoxes = prover->getTriggeredBoxClauses();

    for (auto modalitySubtrie : subtrieMap) {
        // Handle each modality
        if (triggeredDiamonds[modalitySubtrie.first].size() == 0) {
            // If there are no triggered diamonds of a certain modality we can skip it
            cout << " NO CHILDREN" << endl;
            continue;
        }

        Solution childSolution;
        TrieformProverK* childNode = dynamic_cast<TrieformProverK*>(modalitySubtrie.second.get());

            if (isSubsetOf(triggeredDiamonds[modalitySubtrie.first],
                        triggeredBoxes[modalitySubtrie.first])) {
                // The fired diamonds are a subset of the boxes - we thus can create one
                // world.
                //cout << "Depth :" << depth << " " << "Create 1" << endl;


                childSolution =
                    childNode->prove(triggeredBoxes[modalitySubtrie.first]);

                if (childSolution.satisfiable) {
                    continue;
                }
            } else {



        // The fired diamonds are not a subset of the fired boxes, we need to
        // create one world for each diamond clause
        bool diamondFailed = false;

        cout << "MODEL: ";
        for (auto lit : currentModel) {
            cout << lit.toString() << " ";
        }
        cout << endl;
        diamond_queue diamondPriority =
            prover->getPrioritisedTriggeredDiamonds(modalitySubtrie.first, triggeredBoxes[modalitySubtrie.first], triggeredDiamonds[modalitySubtrie.first]);
        while (!diamondPriority.empty()) {
            // Create a world for each diamond
            Literal diamond = diamondPriority.top().literal;
            diamondPriority.pop();

            literal_set childAssumptions =
                literal_set(triggeredBoxes[modalitySubtrie.first]);
            childAssumptions.insert(diamond);

            // Run the solver for the next level
            childSolution = childNode->prove(childAssumptions);

            // Clause propagation
            /*
            bool shouldRestart = false;
            for (literal_set learnClause : prover->getClauses(modalitySubtrie.first, prover->negatedClauses(childNode->allConflicts))) {
                for (auto x : learnClause) cout << x.toString() << " "; cout << endl;
                allConflicts.push_back(learnClause);
                prover->addClause(learnClause);
                shouldRestart |= clauseConflictsWithModel(learnClause, currentModel);
            }
            if (shouldRestart) {
                return prove(assumptions);
            }
            childNode->allConflicts.clear();
            */

            // If it is satisfiable create the next world
            if (childSolution.satisfiable) {
                // Don't check any more if we only required one
                if (isSubsetOf(triggeredDiamonds[modalitySubtrie.first], 
                            triggeredBoxes[modalitySubtrie.first])) break;
                else continue;
            }
            diamondFailed = true;
            break;
        }

        if (!diamondFailed) continue;
            }

        cout << "FAILURE HAPPENED: " << endl;
        for (auto x : childSolution.conflict) cout << x.toString() << " "; cout << endl;
        for (literal_set learnClause : prover->getClauses(modalitySubtrie.first, childSolution.conflict)) {
            cout << "Learn clause: ";
            for (auto x : learnClause) cout << x.toString() << " "; cout << endl;
            allConflicts.push_back(learnClause);
            prover->addClause(learnClause);
        }
        return prove(assumptions);
    }
    // If we reached here the solution is satisfiable under all modalities
    //updateSolutionMemo(assumptionsBitset, solution);
    return solution;
}



bool TrieformProverK::clauseConflictsWithModel(literal_set clause, literal_set model) {
    bool contains = false;
    for (Literal x : clause) {
        if (model.find(x) != model.end()) {
            contains = true;
            break;
        }
    }
    if (contains) {
        return false;
    }
    return true;
}
