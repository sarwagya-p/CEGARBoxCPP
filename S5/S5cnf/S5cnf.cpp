#include "S5cnf.hpp"

bool isPropLiteral(shared_ptr<Formula> formula){
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

bool isLiteral(shared_ptr<Formula> formula){
    switch (formula->getType())
    {
    case FDiamond: {
        Diamond* dia_subf = dynamic_cast<Diamond*>(formula.get());

        return isPropLiteral(dia_subf->getSubformula());
    }

    case FBox: {
        Box* box_subf = dynamic_cast<Box*>(formula.get());

        if (isPropLiteral(box_subf->getSubformula())) return true;
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

formula_set CNF_to_formula_clauses(CNF_form cnf_formula){
    formula_set clauses;

    for (formula_set clause: cnf_formula){
        if (clause.size() == 1){
            clauses.insert(*(clause.begin()));
            continue;
        }

        clauses.insert(Or::create(clause));
    }
    return clauses;
}

CNF_form convert_CNF(shared_ptr<Formula> d1_formula){
    switch (d1_formula->getType()){
    
    case FAnd: {
        CNF_form cnfSet;

        And* and_f = dynamic_cast<And*>(d1_formula.get());

        for (shared_ptr<Formula> subformula: and_f->getSubformulas()){
            CNF_form subformula_cnf = convert_CNF(subformula);

            cnfSet.insert(cnfSet.end(), subformula_cnf.begin(), subformula_cnf.end());
        }

        return cnfSet;
    } break;

    case FOr: {
        formula_set mainClause;
        CNF_form newClauses;

        Or* or_f = dynamic_cast<Or*>(d1_formula.get());
        
        for (shared_ptr<Formula> subformula: or_f->getSubformulas()){
            if (isLiteral(subformula)){
                mainClause.insert(subformula);
                continue;
            }

            CNF_form subformula_cnf = convert_CNF(subformula);

            // Create new Literal z, add ~z to every clause in subformula_cnf
            // Add subformula_cnf to newClauses, add z to mainClause
        }

        newClauses.push_back(mainClause);
        return newClauses;
    } break;

    default:
        return {{d1_formula}};
        break;
    }
}