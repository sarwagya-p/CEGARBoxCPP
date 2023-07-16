#include "S5Formula.h"

NewVariableGenerator::NewVariableGenerator(const string prepend_){
    prepend = prepend_;
    current = 0;
}

shared_ptr<Formula> NewVariableGenerator::operator()(){
    return Atom::create("$" + prepend + to_string(current++));
}

bool isPropLiteral(shared_ptr<Formula> formula){
    switch (formula->getType()){
            case FAtom:
            case FTrue:
            case FFalse:
            case FNot:
                return true;
                break;

            default:
                return false;
                break;
        }
}

bool isS5Literal(shared_ptr<Formula> formula){
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


CNF_form convert_to_CNF(shared_ptr<Formula> d1_formula, NewVariableGenerator& GetNewVar){
    switch (d1_formula->getType()){
    
    case FAnd: {
        CNF_form cnfSet;

        And* and_f = dynamic_cast<And*>(d1_formula.get());

        for (shared_ptr<Formula> subformula: and_f->getSubformulas()){
            CNF_form subformula_cnf = convert_to_CNF(subformula, GetNewVar);

            cnfSet.insert(cnfSet.end(), subformula_cnf.begin(), subformula_cnf.end());
        }

        return cnfSet;
    } break;

    case FOr: {
        formula_set mainClause;
        CNF_form newClauses;

        Or* or_f = dynamic_cast<Or*>(d1_formula.get());
        
        for (shared_ptr<Formula> subformula: or_f->getSubformulas()){
            if (isS5Literal(subformula)){
                mainClause.insert(subformula);
                continue;
            }

            CNF_form subformula_cnf = convert_to_CNF(subformula, GetNewVar);
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

CNF_form DepthReduceBoxFromCNF(CNF_form cnf_subf, NewVariableGenerator& GetNewVar, int modality){
    CNF_form total_cnf_formula;

    for (formula_set subf_clause: cnf_subf) {
        formula_set propClause;

        vector<CNF_form> auxilary_cnf_formulas;
        for (shared_ptr<Formula> literal: subf_clause){
            switch (literal->getType()){
            case FBox:
            case FDiamond:
                auxilary_cnf_formulas.push_back(DepthReduce(literal, GetNewVar));
                break;

            default:
                propClause.insert(literal);
                break;
            }
        }

        if (!propClause.empty()) auxilary_cnf_formulas.push_back({{Box::create(modality, 1, Or::create(propClause))}});

        if (auxilary_cnf_formulas.size() == 1){
            total_cnf_formula.insert(total_cnf_formula.end(), auxilary_cnf_formulas[0].begin(), auxilary_cnf_formulas[0].end());
            continue;
        }
        formula_set mainClause;

        for (CNF_form cnf_aux: auxilary_cnf_formulas){
            if (cnf_aux.size() == 1) mainClause.insert(cnf_aux[0].begin(),cnf_aux[0].end());
            else {
                shared_ptr<Formula> z = GetNewVar();
                mainClause.insert(Not::create(z));

                for (formula_set aux_clause: cnf_aux){
                    aux_clause.insert(z);
                    total_cnf_formula.push_back(aux_clause);
                }
            }
        }
        if (!mainClause.empty()) total_cnf_formula.push_back(mainClause);
    }

    return total_cnf_formula;
}

CNF_form DepthReduceBox(shared_ptr<Formula> inp_formula, NewVariableGenerator& GetNewVar){
    Box* box_f = dynamic_cast<Box*>(inp_formula.get());
    CNF_form cnf_subf = convert_to_CNF(box_f->getSubformula(), GetNewVar);

    return DepthReduceBoxFromCNF(cnf_subf, GetNewVar, box_f->getModality());
}

CNF_form DepthReduceDiamond(
    shared_ptr<Formula> inp_formula, NewVariableGenerator& GetNewVar){
    
    Diamond* dia_f = dynamic_cast<Diamond*>(inp_formula.get());
    switch (dia_f->getSubformula()->getType())
    {
    case FBox:
    case FDiamond:
        return DepthReduce(dia_f->getSubformula(), GetNewVar);

    case FOr: {
        Or* or_subf = dynamic_cast<Or*>(dia_f->getSubformula().get());
        vector<CNF_form> auxilary_cnf_formulas;

        for (shared_ptr<Formula> or_operand: or_subf->getSubformulas()){
            auxilary_cnf_formulas.push_back(DepthReduce(Diamond::create(dia_f->getModality(), 1, or_operand), GetNewVar));
        }

        formula_set mainClause;
        CNF_form auxClauses;

        for (CNF_form& aux_cnf: auxilary_cnf_formulas){
            if (aux_cnf.size() == 1) {
                mainClause.insert(aux_cnf[0].begin(), aux_cnf[0].end());
                continue;
            }

            shared_ptr<Formula> z = GetNewVar();

            mainClause.insert(Not::create(z));

            for (formula_set aux_clause: aux_cnf){
                aux_clause.insert(z);
                auxClauses.push_back(aux_clause);
            }
        }

        auxClauses.push_back(mainClause);
        return auxClauses;
    } break;

    case FAnd: {
        And* and_f = dynamic_cast<And*>(dia_f->getSubformula().get());

        CNF_form modal_subf_cnfs;
        formula_set prop_subf;

        for (shared_ptr<Formula> subf: and_f->getSubformulas()){
            switch (subf->getType())
            {
            case FBox:
            case FDiamond:{
                CNF_form cnf_subformula = DepthReduce(subf, GetNewVar);
                modal_subf_cnfs.insert(modal_subf_cnfs.end(), cnf_subformula.begin(), cnf_subformula.end());
            } break;
            
            default:
                prop_subf.insert(subf);
                break;
            }
        }
        if (prop_subf.empty()) return modal_subf_cnfs;
        if (prop_subf.size() == 1){
            CNF_form cnf_subformula = DepthReduce(Diamond::create(dia_f->getModality(), 1, *(prop_subf.begin())), GetNewVar);
            modal_subf_cnfs.insert(modal_subf_cnfs.end(), cnf_subformula.begin(), cnf_subformula.end());
            return modal_subf_cnfs;
        }

        CNF_form cnf_subf = convert_to_CNF(And::create(prop_subf), GetNewVar);
        shared_ptr<Formula> z = GetNewVar();

        for (formula_set& subf_clause: cnf_subf){
            subf_clause.insert(z);
        }
        modal_subf_cnfs.push_back({Diamond::create(dia_f->getModality(), 1, z)});
        CNF_form auxilary_box = DepthReduceBoxFromCNF(cnf_subf, GetNewVar, dia_f->getModality());
        
        modal_subf_cnfs.insert(modal_subf_cnfs.end(), auxilary_box.begin(), auxilary_box.end());
        return modal_subf_cnfs;
    } break;
    
    default:
        return {{Diamond::create(dia_f->getModality(), 1, dia_f->getSubformula())}};
        break;
    }

    CNF_form cnf_subf = convert_to_CNF(dia_f->getSubformula(), GetNewVar);

    shared_ptr<Formula> z = GetNewVar();

    for (formula_set& clause: cnf_subf){
        clause.insert(z);
    }

    CNF_form auxilary_box_cnf = DepthReduceBoxFromCNF(cnf_subf, GetNewVar, dia_f->getModality());
    auxilary_box_cnf.push_back({Diamond::create(dia_f->getModality(), 1, Not::create(z))});

    return auxilary_box_cnf;
}

CNF_form DepthReduceAnd(shared_ptr<Formula> inp_formula, NewVariableGenerator& GetNewVar){
    And* and_f = dynamic_cast<And*>(inp_formula.get());

    CNF_form cnf_formula;
    for (shared_ptr<Formula> subformula: and_f->getSubformulas()){
        CNF_form cnf_subformula = DepthReduce(subformula, GetNewVar);

        cnf_formula.insert(cnf_formula.end(), cnf_subformula.begin(), cnf_subformula.end());
    }
    return cnf_formula;
}

CNF_form DepthReduceOr(shared_ptr<Formula> inp_formula, NewVariableGenerator& GetNewVar){
    Or* or_f = dynamic_cast<Or*>(inp_formula.get());

    formula_set mainClause;
    CNF_form newClauses;

    for (shared_ptr<Formula> subformula: or_f->getSubformulas()){
        CNF_form cnf_subformula = DepthReduce(subformula, GetNewVar);

        shared_ptr<Formula> z = GetNewVar();

        mainClause.insert(Not::create(z));

        for (formula_set& clause: cnf_subformula){
            clause.insert(z);

            newClauses.push_back(clause);
        }
    }
    newClauses.push_back(mainClause);
    return newClauses;
}

CNF_form DepthReduce(
    shared_ptr<Formula> inp_formula, NewVariableGenerator& GetNewVar){
    switch (inp_formula->getType())
    {
    case FBox:
        return DepthReduceBox(inp_formula, GetNewVar);
        break;

    case FDiamond:
        return DepthReduceDiamond(inp_formula, GetNewVar);
        break;

    case FAnd:
        return DepthReduceAnd(inp_formula, GetNewVar);
        break;

    case FOr:
        return DepthReduceOr(inp_formula, GetNewVar);
        break;

    default:
        return {{inp_formula}};
        break;
    }
}

string clauseToString(formula_set clause){
    if (clause.size() == 1){
        shared_ptr<Formula> lit = *(clause.begin());

        return lit->toString();
        
    }

    string s = "( ";
    for (shared_ptr<Formula> lit: clause) s += lit->toString() + " | ";
    s = s.substr(0, s.size()-3);
    s += ")";
    return s;
}
string cnfToString(CNF_form cnf_formula){
    string s;
    for (formula_set clause: cnf_formula){
        s+= clauseToString(clause);
        s += " & ";
    }
    s = s.substr(0, s.size()-3);
    s+="\n";
    return s;
}