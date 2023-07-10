#include "S5Formula/S5Formula.h"
#include "../ParseFormula/ParseFormula.h"

int main(){
    string file = "inp.txt";
    ParseFormula parser(&file);

    shared_ptr<Formula> formula = parser.parseFormula();

    NewVariableGenerator GetNewVar("depth");

    CNF_form reduced_formula = S5FormulaSimplifier(formula).DepthReduce(formula, GetNewVar);

    cout << S5FormulaSimplifier(formula).cnfToString(reduced_formula) << endl;
}