#include "S5Formula.hpp"

NewVariableGenerator::NewVariableGenerator(string prepend_=""){
    prepend = prepend_;
    current = 0;
}

shared_ptr<Formula> NewVariableGenerator::operator()(){
    return Atom::create("$" + prepend + to_string(current));
    current++;
}


bool S5FormulaSimplifier::isPropLiteral(shared_ptr<Formula> formula){
    switch (formula->getType()){
            case FAtom:
                return true;
                break;

            case FTrue:
                return true;
                break;
            
            case FFalse:
                return true;
                break;

            default:
                return false;
                break;
        }
}

bool S5FormulaSimplifier::isS5Literal(shared_ptr<Formula> formula){
    switch (formula->getType())
    {
    case FDiamond: {
        Diamond* dia_subf = dynamic_cast<Diamond*>(formula.get());

        return dia_subf->getPower() == 1 && S5FormulaSimplifier::isPropLiteral(dia_subf->getSubformula());
    }

    case FBox: {
        Box* box_subf = dynamic_cast<Box*>(formula.get());

        if (box_subf->getPower() != 1) return false;

        if (S5FormulaSimplifier::isPropLiteral(box_subf->getSubformula())) return true;
        if (box_subf->getSubformula()->getType() != FOr) return false;

        Or* or_subf = dynamic_cast<Or*>(box_subf->getSubformula().get());

        for (shared_ptr<Formula> it: or_subf->getSubformulas()){
            if (!isPropLiteral(it)) return false;
        }
        return true;
    }
    
    default:
        return isPropLiteral(formula);
        break;
    }
}


CNF_form S5FormulaSimplifier::convert_to_CNF(shared_ptr<Formula> d1_formula, NewVariableGenerator GetNewVar=NewVariableGenerator()){
    switch (d1_formula->getType()){
    
    case FAnd: {
        CNF_form cnfSet;

        And* and_f = dynamic_cast<And*>(d1_formula.get());

        for (shared_ptr<Formula> subformula: and_f->getSubformulas()){
            CNF_form subformula_cnf = S5FormulaSimplifier::convert_to_CNF(subformula);

            cnfSet.insert(cnfSet.end(), subformula_cnf.begin(), subformula_cnf.end());
        }

        return cnfSet;
    } break;

    case FOr: {
        formula_set mainClause;
        CNF_form newClauses;

        Or* or_f = dynamic_cast<Or*>(d1_formula.get());
        
        for (shared_ptr<Formula> subformula: or_f->getSubformulas()){
            if (S5FormulaSimplifier::isS5Literal(subformula)){
                mainClause.insert(subformula);
                continue;
            }

            CNF_form subformula_cnf = S5FormulaSimplifier::convert_to_CNF(subformula);
            shared_ptr<Formula> z = GetNewVar();
            
            for (formula_set clause: subformula_cnf){
                clause.insert(z);

                newClauses.push_back(clause);
            }
            mainClause.insert(Not::create(z));
        }

        newClauses.push_back(mainClause);
        return newClauses;
    } break;

    default:
        return {{d1_formula}};
        break;
    }
}

CNF_form S5FormulaSimplifier::DepthReduceBox(shared_ptr<Formula> inp_formula, NewVariableGenerator GetNewVar=NewVariableGenerator()){
    Box* box_f = dynamic_cast<Box*>(inp_formula.get());

    switch (box_f->getSubformula()->getType())
    {
    case FBox:
        return S5FormulaSimplifier::DepthReduce(box_f->getSubformula(), GetNewVar);
        break;
    case FDiamond:
        return DepthReduce(box_f->getSubformula(), GetNewVar);
        break;
    
    case FAnd:{
        CNF_form cnf_formula;

        And* and_subf = dynamic_cast<And*>(box_f->getSubformula().get());

        for (shared_ptr<Formula> subf: and_subf->getSubformulas()){
            CNF_form cnf_subformula = DepthReduce(Box::create(box_f->getModality(), 1, subf));

            cnf_formula.insert(cnf_formula.end(), cnf_subformula.begin(), cnf_formula.end());
        }

        return cnf_formula;
        }

    default:
        return {{inp_formula}};
        break;
    }
}

CNF_form S5FormulaSimplifier::DepthReduce(
    shared_ptr<Formula> inp_formula, NewVariableGenerator GetNewVar=NewVariableGenerator()){
        
    switch (inp_formula->getType())
    {
    case FBox:
        return S5FormulaSimplifier::DepthReduceBox(inp_formula, GetNewVar);
        break;

    case FDiamond:
        return S5FormulaSimplifier::DepthReduceDiamond(inp_formula, GetNewVar);
        break;

    case FAnd:
        return S5FormulaSimplifier::DepthReduceAnd(inp_formula, GetNewVar);

    case FOr: 
        return S5FormulaSimplifier::DepthReduceOr(inp_formula, GetNewVar);
        break;

    default:
        return {{inp_formula}};
        break;
    }
}