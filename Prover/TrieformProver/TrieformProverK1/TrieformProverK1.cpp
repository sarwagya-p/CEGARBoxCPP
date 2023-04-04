#include "TrieformProverK1.h"

shared_ptr<Prover> TrieformProverK1::completeProver = make_shared<IpasirProver>();

shared_ptr<Trieform>
TrieformFactory::makeTrieK1(const shared_ptr<Formula> &formula,
        shared_ptr<Trieform> trieParent) {
    shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverK1());
    trie->initialise(formula, trieParent);
    return trie;
}
shared_ptr<Trieform>
TrieformFactory::makeTrieK1(const shared_ptr<Formula> &formula,
        const vector<int> &newModality,
        shared_ptr<Trieform> trieParent) {
    shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverK1());
    trie->initialise(formula, newModality, trieParent);
    return trie;
}
shared_ptr<Trieform>
TrieformFactory::makeTrieK1(const vector<int> &newModality,
        shared_ptr<Trieform> trieParent) {
    shared_ptr<Trieform> trie = shared_ptr<Trieform>(new TrieformProverK1());
    trie->initialise(newModality, trieParent);
    return trie;
}

TrieformProverK1::TrieformProverK1() {}
TrieformProverK1::~TrieformProverK1() {}

shared_ptr<Trieform>
TrieformProverK1::create(const shared_ptr<Formula> &formula) {
    return TrieformFactory::makeTrieK1(formula, shared_from_this());
}
shared_ptr<Trieform> TrieformProverK1::create(const shared_ptr<Formula> &formula,
        const vector<int> &newModality) {
    return TrieformFactory::makeTrieK1(formula, newModality, shared_from_this());
}
shared_ptr<Trieform> TrieformProverK1::create(const vector<int> &newModality) {
    shared_ptr<Trieform> test = shared_from_this();
    return TrieformFactory::makeTrieK1(newModality, shared_from_this());
}

shared_ptr<Bitset>
TrieformProverK1::convertAssumptionsToBitset(literal_set literals) {
    shared_ptr<Bitset> bitset =
        shared_ptr<Bitset>(new Bitset(2 * assumptionsSize));
    for (Literal literal : literals) {
        bitset->set(2 * idMap[literal.getName()] + literal.getPolarity());
    }
    return bitset;
}

void TrieformProverK1::updateSolutionMemo(const shared_ptr<Bitset> &assumptions,
        Solution solution) {
    if (solution.satisfiable) {
        localMemo.insertSat(assumptions);
    } else {
        localMemo.insertUnsat(assumptions, solution.conflict);
    }
}

void TrieformProverK1::preprocess() {}

void TrieformProverK1::prepareSAT(name_set extra) {
    for (string name : extra) {
        idMap[name] = assumptionsSize++;
    }
    modal_names_map modalExtras = completeProver->prepareSAT(clauses, extra);
    for (auto modalSubtrie : subtrieMap) {
        modalSubtrie.second->prepareSAT(modalExtras[modalSubtrie.first]);
    }
}

shared_ptr<Bitset>
TrieformProverK1::fleshedOutAssumptionBitset(literal_set model) {
    shared_ptr<Bitset> bitset =
        shared_ptr<Bitset>(new Bitset(2 * assumptionsSize)); 
    for (auto x : idMap) {
        Literal lit = Literal{x.first, 0};
        bitset->set(2 * x.second + (model.find(lit) == model.end()));
    }
    return bitset;
}

bool TrieformProverK1::isSatisfiable() {
    literal_set assumps;
    assumps.insert(Literal("$root", true));
    return prove(assumps).satisfiable;
}

Solution TrieformProverK1::prove(literal_set assumptions) {
    return prove(0, assumptions);
}

Solution TrieformProverK1::prove(int depth, literal_set assumptions) {
    // Check solution memo
    cout << "Depth: " << depth << " Proving: ";
    for (auto x : assumptions) cout << x.toString() << " "; cout << endl;
    shared_ptr<Bitset> assumptionsBitset =
        convertAssumptionsToBitset(assumptions);
    LocalSolutionMemoResult memoResult = localMemo.getFromMemo(assumptionsBitset);

    if (memoResult.inSatMemo) {
        return memoResult.result;
    }

    // Solve locally
    Solution solution = completeProver->solve(assumptions);

    if (!solution.satisfiable) {
        // completeProver->reduce_conflict(solution.conflict);
        updateSolutionMemo(assumptionsBitset, solution);
        return solution;
    }

    literal_set currentModel = completeProver->getModel();
    assumptionsBitset = fleshedOutAssumptionBitset(currentModel);

    completeProver->calculateTriggeredDiamondsClauses();
    modal_literal_map triggeredDiamonds = completeProver->getTriggeredDiamondClauses();
    completeProver->calculateTriggeredBoxClauses();
    modal_literal_map triggeredBoxes = completeProver->getTriggeredBoxClauses();

    for (auto modalitySubtrie : subtrieMap) {
        // Handle each modality
        if (triggeredDiamonds[modalitySubtrie.first].size() == 0) {
            // If there are no triggered diamonds of a certain modality we can skip it
            continue;
        }

        Solution childSolution;
        TrieformProverK1* childNode = dynamic_cast<TrieformProverK1*>(modalitySubtrie.second.get());

        // The fired diamonds are not a subset of the fired boxes, we need to
        // create one world for each diamond clause
        bool diamondFailed = false;

        diamond_queue diamondPriority =
            completeProver->getPrioritisedTriggeredDiamonds(modalitySubtrie.first, triggeredBoxes[modalitySubtrie.first], triggeredDiamonds[modalitySubtrie.first]);

        while (!diamondPriority.empty()) {
            // Create a world for each diamond
            Literal diamond = diamondPriority.top().literal;
            diamondPriority.pop();

            literal_set childAssumptions =
                literal_set(triggeredBoxes[modalitySubtrie.first]);
            childAssumptions.insert(diamond);

            // Run the solver for the next level
            childSolution = childNode->prove(depth+1, childAssumptions);

            // Clause propagation
            /*
            bool shouldRestart = false;
            for (literal_set learnClause : completeProver->getClauses(modalitySubtrie.first, completeProver->negatedClauses(childNode->allConflicts))) {
                for (auto x : learnClause) cout << x.toString() << " "; cout << endl;
                allConflicts.push_back(learnClause);
                completeProver->addClause(learnClause);
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
        
        cout << "Depth: " << depth << "Failed: " << endl; 
        for (literal_set learnClause : completeProver->getClauses(modalitySubtrie.first, childSolution.conflict)) {
            allConflicts.push_back(learnClause);
            for (auto x : learnClause) cout << x.toString() << " "; cout << endl;
            completeProver->addClause(learnClause);
        }
        return prove(depth, assumptions);
    }
    // If we reached here the solution is satisfiable under all modalities
    cout << "Depth: " << depth << " SAT";
    cout << "MODEL: ";
    for (auto x : currentModel) cout << x.toString() << " "; cout << endl;
    updateSolutionMemo(assumptionsBitset, solution);
    return solution;
}



bool TrieformProverK1::clauseConflictsWithModel(literal_set clause, literal_set model) {
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
