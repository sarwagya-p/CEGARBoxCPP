#include "IpasirProver.h"

shared_ptr<Lingeling> IpasirProver::completeSolver = make_shared<Lingeling>();
IpasirProver::IpasirProver(bool onesat) {
  // solver->random_var_freq = 0;
  // solver->rnd_init_act = true;
  // solver->ccmin_mode = 1;
  // solver->clause_decay = 0;
  // solver->var_decay = 0.5;
  // solver->luby_restart = false;
  // solver->
  // solver->eliminate(true);
  if (onesat) {
    calcSolver = completeSolver;

  } else {
      calcSolver = solver;
  }
}
IpasirProver::~IpasirProver() {}

modal_names_map IpasirProver::prepareSAT(FormulaTriple clauses,
                                          name_set extra) {
  prepareExtras(extra);

  modal_names_map newExtra;
  prepareModalClauses(clauses.getBoxClauses(), newExtra, boxLits, boxFromRight);
  prepareModalClauses(clauses.getDiamondClauses(), newExtra, diamondLits,
                      diamondFromRight);

  prepareFalse();
  prepareClauses(clauses.getClauses());
  return newExtra;
}

void IpasirProver::prepareFalse() {
  solver->add_clause(vector<int> {-createOrGetVariable("$false")});
}

void IpasirProver::prepareExtras(name_set extra) {
  for (string name : extra) {
    createOrGetVariable(name);
  }
}

void IpasirProver::prepareClauses(clause_set clauses) {
  for (Clause clause : clauses) {
    if (clause.formula->getType() == FOr) {
      vector<int> literals;
      for (shared_ptr<Formula> subformula :
           dynamic_cast<Or *>(clause.formula.get())->getSubformulas()) {
        literals.push_back(makeLiteral(subformula));
      }
      solver->add_clause(literals);
    } else {
      solver->add_clause(vector<int> {makeLiteral(clause.formula)});
    }
  }
}

void IpasirProver::prepareModalClauses(modal_clause_set modal_clauses,
                                        modal_names_map &newExtra,
                                        modal_lit_implication &modalLits,
                                        modal_lit_implication &modalFromRight) {
  for (ModalClause clause : modal_clauses) {
    if (clause.left->getType() == FAtom) {
      createOrGetVariable(getPrimitiveName(clause.left));
    } else if (clause.left->getType() == FNot) {
      createOrGetVariable(getPrimitiveName(clause.left));
    }
    newExtra[clause.modality].insert(getPrimitiveName(clause.right));
    createModalImplication(clause.modality, toLiteral(clause.left),
                           toLiteral(clause.right), modalLits, modalFromRight);
  }
}

int IpasirProver::createOrGetVariable(string name) {
  if (variableMap.find(name) == variableMap.end()) {
    variableMap[name] = variableMap.size()+1;
    nameMap[variableMap[name]] = name;
  }
  return variableMap[name];
}

int IpasirProver::makeLiteral(shared_ptr<Formula> formula) {
  if (formula->getType() == FAtom) {
    string name = dynamic_cast<Atom *>(formula.get())->getName();
    return createOrGetVariable(name);
  } else if (formula->getType() == FNot) {
    string name = dynamic_cast<Atom *>(
                      dynamic_cast<Not *>(formula.get())->getSubformula().get())
                      ->getName();
    return -createOrGetVariable(name);
  }
  throw invalid_argument("Expected Atom or Not but got " + formula->toString());
}

shared_ptr<vector<int>>
IpasirProver::convertAssumptions(literal_set assumptions) {
  shared_ptr<vector<int>> literals (new vector<int> ());
  for (Literal assumption : assumptions) {
    int variable = variableMap[assumption.getName()];
    literals->push_back(assumption.getPolarity() ? variable 
                                            : -variable);
  }
  return literals;
}

void IpasirProver::reduce_conflict(literal_set& conflict) {
    literal_set all_lits = conflict;
    for (auto lit_to_remove : all_lits) {
        literal_set new_conflict;
        for (auto x : conflict) if (x != lit_to_remove) new_conflict.insert(x);
        if (new_conflict.size() < conflict.size()) {
            Solution sol = solve(new_conflict);
            if (!sol.satisfiable) {
                conflict = sol.conflict;
            }
        }
    }
}

bool IpasirProver::modelSatisfiesAssump(Literal assumption) {
  if (variableMap.find(assumption.getName()) == variableMap.end()) {
    return false;
  }
  int val = variableMap[assumption.getName()];
  int res = solver->get_model(val, val)[0];
  if (res == 0) {
    throw runtime_error("Model value for " + assumption.getName() +
                        " is undefined");
  }
  return ((res>0) && assumption.getPolarity()) ||
         ((res<0) && !assumption.getPolarity());
}

literal_set IpasirProver::convertConflictToAssumps(
    vector<int> conflictLits) {
  literal_set conflict;
  for (int i = 0; i < conflictLits.size(); i++) {
    conflict.insert(Literal(nameMap[abs(conflictLits[i])],
                            conflictLits[i]>0));
  }
  return conflict;
}

Solution IpasirProver::solve(const literal_set &assumptions) {
  Solution solution;
  shared_ptr<vector<int>> vecAssumps =
      convertAssumptions(assumptions);
  /*
  cout << "SOLVING: " << endl;
  for (auto x : assumptions) cout << x.toString() << " ";
  cout << endl;
  */
  solution.satisfiable = solver->solve(*vecAssumps);
  if (!solution.satisfiable) {
    solution.conflict = convertConflictToAssumps(solver->used_assumptions());
    /*
      cout << "CONFLICT: " << endl;
      for (auto x : solution.conflict) cout << x.toString() << " ";
      cout << endl;
      */
  } else {
  }
  return solution;
}

void IpasirProver::addClause(literal_set clause) {
  solver->add_clause(*convertAssumptions(clause));
}

int IpasirProver::getLiteralId(Literal literal) {
  return variableMap[literal.getName()];
}

void IpasirProver::printModel() {
    cout << "NOT IMPLEMENTED" << endl;
}

literal_set IpasirProver::getModel() {
    literal_set model;
    for (auto x : solver->get_model()) {
        model.insert(
            Literal(nameMap[abs(x)], x > 0)
        );
    }
    return model;
    
}
