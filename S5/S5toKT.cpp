#include "S5toKT.h"
#include<iostream>

shared_ptr<Formula> reduceS5ToKT(shared_ptr<Formula> inp_formula){
    NewVariableGenerator GetNewVar("$s5red");

    CNF_form cnf_formula = DepthReduce(inp_formula, GetNewVar);

    formula_set clauses;

    for (formula_set clause: cnf_formula){
        shared_ptr<Formula> orClause = Or::create(clause);
        clauses.insert(orClause);
    }

    return And::create(clauses);
}