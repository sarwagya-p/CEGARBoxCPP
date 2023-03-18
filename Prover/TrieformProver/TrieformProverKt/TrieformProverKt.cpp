#include "TrieformProverKt.h"

map<vector<int>, shared_ptr<TrieformProverKt>> TrieformProverKt::all_trieforms;
ProbationSolutionMemo TrieformProverKt::probationMemo;

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

shared_ptr<Bitset>
TrieformProverKt::fleshedOutAssumptionBitset(literal_set model) {
    shared_ptr<Bitset> bitset =
        shared_ptr<Bitset>(new Bitset(2 * assumptionsSize)); 
    for (auto x : idMap) {
        Literal lit = Literal{x.first, 0};
        bitset->set(2 * x.second + (model.find(lit) == model.end()));
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

shared_ptr<TrieformProverKt> TrieformProverKt::convertToGrid(vector<int>& modal_context) {
    if (all_trieforms.find(modal_context) == all_trieforms.end()) {
        all_trieforms[modal_context] = shared_ptr<TrieformProverKt>(new TrieformProverKt);
        all_trieforms[modal_context]->modalContext = make_shared<vector<int>>(modal_context);
    }     

    shared_ptr<TrieformProverKt> gridTrieform = all_trieforms[modal_context];

    gridTrieform->clauses.extendClauses(getClauses());

    for (auto modalSubtrie: subtrieMap) {

        modal_context[abs(modalSubtrie.first)] += (modalSubtrie.first > 0) ? 1 : -1;
        shared_ptr<TrieformProverKt> gridTrieformSuccessor = dynamic_cast<TrieformProverKt*>(modalSubtrie.second.get())->convertToGrid(modal_context); 
        modal_context[abs(modalSubtrie.first)] -= (modalSubtrie.first > 0) ? 1 : -1;

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
    if (numRelations == -1) numRelations = 0;
    vector<int> modal_context (numRelations+1);
    auto replacement = convertToGrid(modal_context);
    replacement->numRelations = numRelations;
    return replacement;
    //replacement.buildConnections();
}

bool TrieformProverKt::isSatisfiable() {
    literal_set assumps;
    assumps.insert(Literal("$root", true));
    return prove(0, assumps).satisfiable;
}

Solution TrieformProverKt::prove(literal_set assumptions) {
    return prove(0, assumptions);
}

Solution TrieformProverKt::prove(int depth, literal_set assumptions) {
    ////cout << "Depth :" << depth << " " << endl;
    ////cout << "Depth :" << depth << " " << endl << "SIZES: " << pastModels.size() << " " << history.size() << endl;
    //if (pastModels.size() != history.size()) {
    //    //cout << "Depth :" << depth << " " << "WTF" << endl;
    //}
    ProbationSolutionMemoState probationState = probationMemo.getState();

    //cout << endl;
    //cout << "Depth :" << depth << " " << "PROVE CONTEXT\n" << endl;
    for  (auto x : assumptions) {
        //cout <<  x.toString() << " ";
    }
    //cout << endl;
    
    for (auto x : all_trieforms) {
        if (this == x.second.get()) {
            //cout << "Depth :" << depth << " " << "At sum: " << x.first[1] << endl;
        }
    }
    //cout << "Depth :" << depth << " " << "At depth: " << depth << endl;
    //cout << "Depth :" << depth << " " << "History size and depths: " << history.size() << endl;
    for (auto x : history) {
        //cout <<  x.first << " ";
    }
    //cout << endl;

    // Check solution memo
    shared_ptr<Bitset> assumptionsBitset =
        convertAssumptionsToBitset(assumptions);
    LocalSolutionMemoResult memoResult = localMemo.getFromMemo(assumptionsBitset);
    if (memoResult.inSatMemo) {
        //cout << "Depth :" << depth << " " << "sat from actual memo" << endl;
        return memoResult.result;
    }
    
    ProbationSolutionMemoResult probationMemoResult = probationMemo.getFromMemo(modalContext, assumptionsBitset);

    if (probationMemoResult.inSatMemo) {
        //cout << "Depth :" << depth << " " << "sat from probation: " << probationMemo.minimalRoot << endl;
        return probationMemoResult.result;
    }


    int inHistory = isInHistory(history, assumptionsBitset);
    if (inHistory != -1) {
        probationMemo.updateMinimalRoot(inHistory);
        //cout << "Depth :" << depth << " " << "Using history: " << inHistory << endl;
        return {true, literal_set()};
    }

    // Solve locally
    Solution solution = prover->solve(assumptions);

    if (!solution.satisfiable) {
        //updateSolutionMemo(assumptionsBitset, solution);
        // Clear probationCache
        probationMemo.setState(probationState);

        /*
        if (depth == probationMemoTracker.first) {
            for (auto x : probationMemoTracker.second) {
                auto trieform = all_trieforms[x];
                trieform->probationMemo.clear();
            }
            probationMemoTracker.first = -1;
            probationMemoTracker.second.clear();
        }
        */
        //cout << "Depth :" << depth << " " << "Is UNSAT" << endl;
        return solution;
    }
    literal_set currentModel = prover->getModel();
    //cout << "Depth :" << depth << " " << "Current model: ";
    for (auto x : currentModel) {
        //cout << x.toString() << " ";
    }
    //cout << "Depth :" << depth << " " << endl;
    ////cout << "Depth :" << depth << " " << "ACTUAL" << endl;
    assumptionsBitset = fleshedOutAssumptionBitset(currentModel);
    prover->calculateTriggeredDiamondsClauses();
    modal_literal_map triggeredDiamonds = prover->getTriggeredDiamondClauses();

    // Only bother if the number of fired diamonds is non zero 
    if (triggeredDiamonds.size() != 0) {
        ////cout << "Depth :" << depth << " " << "Proving IS SAT (by no diamonds): " << endl;
   

    literal_set triggeredImplications = prover->rememberTriggeredImplications();
    //cout << "Depth :" << depth << " " << "TRIG IMP: ";
    for (auto x : triggeredImplications) {
        //cout << "Depth :" << depth << " " << x.toString() << " ";
    }
    //cout << "Depth :" << depth << " " << endl;
    prover->calculateTriggeredBoxClauses();
    modal_literal_map triggeredBoxes = prover->getTriggeredBoxClauses();
        

    for (auto modalitySubtrie : subtrieMap) {
        // Handle each modality
        ////cout << "Depth :" << depth << " " << "Checking modality: " << modalitySubtrie.first << endl;
        if (triggeredDiamonds[modalitySubtrie.first].size() == 0) {
            // If there are no triggered diamonds of a certain modality we can skip it
            ////cout << "Depth :" << depth << " " << "Skipping: " << endl;
            continue;
        }
        if (isSubsetOf(triggeredDiamonds[modalitySubtrie.first],
                    triggeredBoxes[modalitySubtrie.first])) {
            // The fired diamonds are a subset of the boxes - we thus can create one
            // world.
            ////cout << "Depth :" << depth << " " << "Create 1" << endl;
            
            pastModels.push_back(currentModel);
            history.push_back({depth, assumptionsBitset});
            Solution childSolution =
                dynamic_cast<TrieformProverKt*>(modalitySubtrie.second.get())->prove(depth+1, triggeredBoxes[modalitySubtrie.first]);
            history.pop_back();
            pastModels.pop_back();

            if (childSolution.shouldRestart) {
                //cout << "Depth :" << depth << " " << "Backtracking time" << endl;
                    probationMemo.setState(probationState);

                if (shouldRestart && restartUntil == history.size()) {
                    // restart current node
                    //cout << "Depth :" << depth << " " << "Restarting current node" << history.size() << endl;
                    shouldRestart = false;


                    return prove(depth, assumptions);
                } else {
                    // Keep backtracking until we should restart
                    return childSolution;
                }
            }

            // If the one world solution is satisfiable, then we're all good
            if (childSolution.satisfiable) {
                continue;
            }
            // Otherwise, as the diamonds are a subset of the boxes, the left
            // implications of the problem box clauses cannot be true with any diamond
            // clause of this modality
            vector<literal_set> badImplications = prover->getNotProblemBoxClauses(
                    modalitySubtrie.first, childSolution.conflict, triggeredImplications);
            badImplications.push_back(
                    prover->getNotAllDiamondLeft(modalitySubtrie.first));
            // Add ~leftDiamond=>\/~leftProbemBox
            for (literal_set learnClause : generateClauses(badImplications)) {
                //cout << "Depth :" << depth << " " << "Learning clause 1: ";
                for (auto x :  learnClause) //cout << "Depth :" << depth << " " << x.toString() << " ";
                //cout << "Depth :" << depth << " " << endl;
                prover->addClause(learnClause);
                restartUntil = history.size();
                restartUntil = min(restartUntil, checkClauseAgainstPastModels(learnClause));
            }
        } else {
            bool diamondFailed = false;
            ////cout << "Depth :" << depth << " " << "Create 2" << endl;
            // The fired diamonds are not a subset of the fired boxes, we need to
            // create one world for each diamond clause
            diamond_queue diamondPriority =
                prover->getPrioritisedTriggeredDiamonds(modalitySubtrie.first, triggeredBoxes[modalitySubtrie.first], triggeredDiamonds[modalitySubtrie.first]);

            while (!diamondPriority.empty()) {
                // Create a world for each diamond
                Literal diamond = diamondPriority.top().literal;
                diamondPriority.pop();
                ////cout << "Depth :" << depth << " " << "Checking out child Diamond: " << diamond.toString() << " Remaining: " << diamondPriority.size() << endl;
                literal_set childAssumptions =
                    literal_set(triggeredBoxes[modalitySubtrie.first]);
                childAssumptions.insert(diamond);


                pastModels.push_back(currentModel);
                history.push_back({depth, assumptionsBitset});
                // Run the solver for the next level
                Solution childSolution =
                    dynamic_cast<TrieformProverKt*>(modalitySubtrie.second.get())->prove(depth+1, childAssumptions);
                history.pop_back();
                pastModels.pop_back();

                if (childSolution.shouldRestart) {
                    //cout << "Depth :" << depth << " " << "Backtracking time" << endl;
                    probationMemo.setState(probationState);
                    if (shouldRestart && restartUntil == history.size()) {
                        // restart current node
                        //cout << "Depth :" << depth << " " << "Restarting current node" << history.size() << endl;
                        shouldRestart = false;
                        return prove(depth, assumptions);
                    } else {
                        // Keep backtracking until we should restart


                        return childSolution;
                    }
                }

                // If it is satisfiable create the next world
                if (childSolution.satisfiable) {
                    continue;
                }

                diamondFailed = true;

                // Otherwise there must have been a conflict
                vector<literal_set> badImplications = prover->getNotProblemBoxClauses(
                        modalitySubtrie.first, childSolution.conflict, triggeredImplications);

                //cout << "Depth :" << depth << " " << "Pure conflict: ";
                for (auto x : childSolution.conflict) {
                    //cout << x.toString() << " ";
                }
                //cout << endl;
                if (childSolution.conflict.find(diamond) !=
                        childSolution.conflict.end()) {
                    // The diamond clause, either on its own or together with box clauses,
                    // caused a conflict. We must add diamond implies OR NOT problem
                    // box clauses.
                    prover->updateLastFail(diamond);

                    badImplications.push_back(
                            prover->getNotDiamondLeft(modalitySubtrie.first, diamond, triggeredImplications));

                    for (literal_set learnClause : generateClauses(badImplications)) {
                        //cout << "Depth :" << depth << " " << "Learning clause 2: ";
                        for (auto x :  learnClause) //cout << "Depth :" << depth << " " << x.toString() << " ";
                        //cout << "Depth :" << depth << " " << endl;
                        prover->addClause(learnClause);
                        restartUntil = history.size();
                        restartUntil = min(restartUntil, checkClauseAgainstPastModels(learnClause));
                    }

                    // Find new result
                } else {
                    // Only the box clauses caused a conflict, so we must add each diamond
                    // clause implies OR NOT problem box lefts
                    badImplications.push_back(
                            prover->getNotAllDiamondLeft(modalitySubtrie.first));
                    // Add ~leftDiamond=>\/~leftProbemBox
                    for (literal_set learnClause : generateClauses(badImplications)) {
                        //cout << "Depth :" << depth << " " << "Learning clause 3: ";
                        for (auto x :  learnClause) //cout << "Depth :" << depth << " " << x.toString() << " ";
                        //cout << "Depth :" << depth << " " << endl;
                        prover->addClause(learnClause);
                        restartUntil = history.size();
                        restartUntil = min(restartUntil, checkClauseAgainstPastModels(learnClause));
                    }
                }
                break;
            }
            //cout << "Depth :" << depth << " " << "Diamond failed: " << diamondFailed << endl;
            if (!diamondFailed) continue;
        }
        
        probationMemo.setState(probationState);

        //cout << "Depth :" << depth << " " << "BACKTRACK TIME: " << history.size() << " " << restartUntil << endl;
        if (restartUntil > history.size()) {
            //cout << "Depth :" << depth << " " << "WTF MAN" << endl;
        }
        if (history.size() == restartUntil) {
            return prove(depth, assumptions);
        } else {
            shouldRestart = true;
            return {true, literal_set(), true};
        }
    }
    }
    // If we reached here the solution is satisfiable under all modalities
    //cout << "Depth :" << depth << " " << "Is sat, full model:" << endl;
    for (auto x : currentModel) {
        //cout << x.toString() << " ";
    }
    //cout << "Depth :" << depth << " " << endl;

    

    if (probationMemo.minimalRoot == -1) {
        //cout << "Depth :" << depth << " " << "Going straight to actual cache" << endl;
        for (auto x : assumptions) //cout << x.toString() << " ";
        //cout << endl;
        updateSolutionMemo(assumptionsBitset, solution);
    } else if (depth == probationMemo.minimalRoot) {
        //cout << "Depth :" << depth << " " << "Moving probation cache to actual cache" << endl;
        // Move probation cache to actual cache
        for (auto x : probationMemo.getSatSols()) {
            auto trieform = all_trieforms[*x.first];
            trieform->localMemo.insertSat(x.second);
        }
        probationMemo.setState({-1, 0});
    } else if (depth > probationMemo.minimalRoot) {
        probationMemo.insertSat(modalContext, assumptionsBitset);
    } else {
        //cout << "Depth :" << depth << " " << probationMemo.minimalRoot << " "  << "WTFFF SHOULDN'T BE HERE" << endl;
    }
    //cout << "Depth :" << depth << " " << "Returning solution: " <<  solution.satisfiable << endl;
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

int TrieformProverKt::isInHistory(vector<pair<int, shared_ptr<Bitset>>> history,
        shared_ptr<Bitset> bitset) {
    for (unsigned i = history.size(); i-- > 0; ) {
        if (history[i].second->contains(*bitset))
            return history[i].first;
    }
    return -1;
}

void TrieformProverKt::removeTrueAndFalse() {
    for (auto modTrie : all_trieforms) {
        modTrie.second->clauses.removeTrueAndFalse();
    }
}

void TrieformProverKt::prepareSAT(name_set extra_unused) {
    //cout << "Preparing SAT" << endl;
    for (auto modTrie : all_trieforms) {
        vector<string> clauseComponents = modTrie.second->clauses.toStringComponents();

        vector<int> modalContext = modTrie.first;
        name_set extra;

        if (all_of(modalContext.begin(), modalContext.end(), [](int i) { return i==0; })) {
            extra.insert("$root");
        }

        for (int relation = 1; relation <= numRelations; ++relation) {
            for (int jump : {-1, 1}){
                modalContext[relation] += jump;

                if (all_trieforms.find(modalContext) != all_trieforms.end()) {
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
                }


                modalContext[relation] -= jump;

            }


        }
        for (string name : extra) {
            modTrie.second->idMap[name] = modTrie.second->assumptionsSize++;
        }
        modTrie.second->prover->prepareSAT(modTrie.second->clauses, extra);
    }
}

unsigned int TrieformProverKt::checkClauseAgainstPastModels(literal_set clause) {
    for (unsigned int i = 0; i < pastModels.size(); i++) {
        bool contains = false;
        for (Literal x : clause) {
            if (pastModels[i].find(x) != pastModels[i].end()) {
                contains = true;
                break;
            }
        }
        if (!contains) return i;
    }
    return pastModels.size();
}
