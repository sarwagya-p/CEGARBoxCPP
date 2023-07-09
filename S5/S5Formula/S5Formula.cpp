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
    case FDiamond:
        return true;
        break;

    case FBox:
        return true;
        break;
    
    default:
        return isPropLiteral(formula);
        break;
    }
}


CNF_form S5FormulaSimplifier::convert_to_CNF(shared_ptr<Formula> d1_formula, NewVariableGenerator& GetNewVar){
    switch (d1_formula->getType()){
    
    case FAnd: {
        CNF_form cnfSet;

        And* and_f = dynamic_cast<And*>(d1_formula.get());

        for (shared_ptr<Formula> subformula: and_f->getSubformulas()){
            CNF_form subformula_cnf = S5FormulaSimplifier::convert_to_CNF(subformula, GetNewVar);

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

            CNF_form subformula_cnf = S5FormulaSimplifier::convert_to_CNF(subformula, GetNewVar);
            shared_ptr<Formula> z = GetNewVar();
            
            for (formula_set& clause: subformula_cnf){
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

CNF_form S5FormulaSimplifier::DepthReduceBox(shared_ptr<Formula> inp_formula, NewVariableGenerator& GetNewVar){
    Box* box_f = dynamic_cast<Box*>(inp_formula.get());

    CNF_form cnf_subf = S5FormulaSimplifier::convert_to_CNF(box_f->getSubformula(), GetNewVar);
    CNF_form cnf_formula;

    for (formula_set subf_clause: cnf_subf) {
        formula_set mainClause;

        for (shared_ptr<Formula> literal: subf_clause){
            switch (literal->getType()){
            case FBox:{
                Box* box_lit = dynamic_cast<Box*>(literal.get());

                if (box_lit->getPower() == 1) mainClause.insert(literal);
                else mainClause.insert(Box::create(box_lit->getModality(), 1, box_lit->getSubformula()));
            } break;

            case FDiamond:{
                Diamond* dia_lit = dynamic_cast<Diamond*>(literal.get());

                if (dia_lit->getPower() == 1) mainClause.insert(literal);
                else mainClause.insert(Diamond::create(dia_lit->getModality(), 1, dia_lit->getSubformula()));
            } break;

            default:
                mainClause.insert(Box::create(box_f->getModality(), 1, literal));
            }
        }
        cnf_formula.push_back(mainClause);
    }
    return cnf_formula;
}

CNF_form S5FormulaSimplifier::DepthReduceDiamond(
    shared_ptr<Formula> inp_formula, NewVariableGenerator& GetNewVar){
    Diamond* dia_f = dynamic_cast<Diamond*>(inp_formula.get());

    switch (dia_f->getSubformula()->getType())
    {
    case FFalse:
    case FTrue:
    case FAtom: {
        if (dia_f->getPower() == 1) return {{inp_formula}};
        return {{Diamond::create(dia_f->getModality(), 1, dia_f->getSubformula())}};
    } break;
    
    case FBox:
    case FDiamond: {
        return S5FormulaSimplifier::DepthReduce(dia_f->getSubformula(), GetNewVar);
    } break;

    default: {
        // <>phi = <>z ; [](~z | phi)
        shared_ptr<Formula> z = GetNewVar();

        CNF_form newbox_cnf = S5FormulaSimplifier::DepthReduceBox(
            Box::create(dia_f->getModality(), 1, Or::create({Not::create(z), dia_f->getSubformula()})), GetNewVar);

        newbox_cnf.push_back({Diamond::create(dia_f->getModality(), 1, z)});
    } break;
    }
}

CNF_form S5FormulaSimplifier::DepthReduceAnd(shared_ptr<Formula> inp_formula, NewVariableGenerator& GetNewVar){
    And* and_f = dynamic_cast<And*>(inp_formula.get());

    CNF_form cnf_formula;
    for (shared_ptr<Formula> subformula: and_f->getSubformulas()){
        CNF_form cnf_subformula = S5FormulaSimplifier::DepthReduce(subformula, GetNewVar);

        cnf_formula.insert(cnf_formula.end(), cnf_subformula.begin(), cnf_subformula.end());
    }
    return cnf_formula;
}

CNF_form S5FormulaSimplifier::DepthReduceOr(shared_ptr<Formula> inp_formula, NewVariableGenerator& GetNewVar){
    Or* or_f = dynamic_cast<Or*>(inp_formula.get());

    formula_set mainClause;
    formula_set newClauses;

    for (shared_ptr<Formula> subformula: or_f->getSubformulas()){
        CNF_form cnf_subformula = DepthReduce(subformula, GetNewVar);

        shared_ptr<Formula> z = GetNewVar();

        mainClause.insert(Not::create(z));

        for (formula_set& clause: cnf_subformula){
            clause.insert(z);
        }

    }
}

CNF_form S5FormulaSimplifier::DepthReduce(
    shared_ptr<Formula> inp_formula, NewVariableGenerator& GetNewVar){
        
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