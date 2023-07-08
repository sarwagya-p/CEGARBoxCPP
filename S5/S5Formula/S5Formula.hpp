#ifndef S5FORMULA
#define S5FORMULA

#include "../../Formula/And/And.h"
#include "../../Formula/Atom/Atom.h"
#include "../../Formula/Box/Box.h"
#include "../../Formula/Diamond/Diamond.h"
#include "../../Formula/FEnum/FEnum.h"
#include "../../Formula/Formula/Formula.h"
#include "../../Formula/Or/Or.h"
#include "../../Clausifier/FormulaTriple/FormulaTriple.h"

typedef vector<formula_set> CNF_form;

class NewVariableGenerator {
    string prepend;
    int current;
public:
    NewVariableGenerator(string prepend_ = "");
    shared_ptr<Formula> operator()();
};

class S5FormulaSimplifier {
private:
    int newVarCounter;
public:
    shared_ptr<Formula> formula;
    S5FormulaSimplifier(shared_ptr<Formula> inp_formula, int );

    CNF_form convert_to_CNF(shared_ptr<Formula> inp_formula, NewVariableGenerator GetNewVar=NewVariableGenerator());
    
    CNF_form DepthReduce(shared_ptr<Formula> inp_formula, NewVariableGenerator GetNewVar=NewVariableGenerator());

    std::vector<FormulaTriple> form_triples(CNF_form &cnf_formula);

private:
    CNF_form DepthReduceBox(shared_ptr<Formula> inp_formula, NewVariableGenerator GetNewVar=NewVariableGenerator());
    CNF_form DepthReduceDiamond(shared_ptr<Formula> inp_formula, NewVariableGenerator GetNewVar=NewVariableGenerator());
    CNF_form DepthReduceAnd(shared_ptr<Formula> inp_formula, NewVariableGenerator GetNewVar=NewVariableGenerator());
    CNF_form DepthReduceOr(shared_ptr<Formula> inp_formula, NewVariableGenerator GetNewVar=NewVariableGenerator());
    
    bool isPropLiteral(shared_ptr<Formula> inp_formula);
    bool isS5Literal(shared_ptr<Formula> inp_formula);

};

#endif