#include "TrieformProverS5.h"
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
string TrieformProverS5::cnfToString(CNF_form cnf_formula){
  string s;
  for (formula_set clause: cnf_formula){
    s+= clauseToString(clause);
    s += " & ";
  }
  s = s.substr(0, s.size()-3);
  s+="\n";
  return s;
}

bool TrieformProverS5::isPropLiteral(shared_ptr<Formula> formula){
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

bool TrieformProverS5::isS5Literal(shared_ptr<Formula> formula){
  switch (formula->getType())
  {
  case FDiamond:
  case FBox:
    return true;
    break;
  
  default:
    return isPropLiteral(formula);
    break;
  }
}


CNF_form TrieformProverS5::convertToCNF(shared_ptr<Formula> d1_formula){
  switch (d1_formula->getType()){
  
  case FAnd: {
    CNF_form cnfSet;

    And* and_f = dynamic_cast<And*>(d1_formula.get());

    for (shared_ptr<Formula> subformula: and_f->getSubformulas()){
      CNF_form subformula_cnf = convertToCNF(subformula);

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

      CNF_form subformula_cnf = convertToCNF(subformula);
      shared_ptr<Formula> name = cache->createVariable();
      
      for (formula_set& clause: subformula_cnf){
        clause.insert(Not::create(name)->negatedNormalForm());

        newClauses.push_back(clause);
      }
      mainClause.insert(name);
    }
    
    newClauses.push_back(mainClause);
    return newClauses;
  } break;

  default:
    return {{d1_formula}};
    break;
  }
}

CNF_form TrieformProverS5::DepthReduceBoxFromCNF(CNF_form cnf_subf, int modality){
  CNF_form total_cnf_formula;

  for (formula_set subf_clause: cnf_subf) {
    formula_set propClause;

    vector<CNF_form> auxilary_cnf_formulas;
    for (shared_ptr<Formula> literal: subf_clause){
      switch (literal->getType()){
      case FBox:
      case FDiamond:
        auxilary_cnf_formulas.push_back(DepthReduce(literal));
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
        shared_ptr<Formula> z = cache->createVariable();
        mainClause.insert(Not::create(z)->negatedNormalForm());

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

CNF_form TrieformProverS5::DepthReduceBox(shared_ptr<Formula> inp_formula){
  Box* box_f = dynamic_cast<Box*>(inp_formula.get());
  CNF_form cnf_subf = convertToCNF(box_f->getSubformula());

  return DepthReduceBoxFromCNF(cnf_subf, box_f->getModality());
}

CNF_form TrieformProverS5::DepthReduceDiamond(shared_ptr<Formula> inp_formula){
  
  Diamond* dia_f = dynamic_cast<Diamond*>(inp_formula.get());
  switch (dia_f->getSubformula()->getType())
  {
  case FBox:
  case FDiamond:
    return DepthReduce(dia_f->getSubformula());

  case FOr: {
    Or* or_subf = dynamic_cast<Or*>(dia_f->getSubformula().get());
    vector<CNF_form> auxilary_cnf_formulas;

    for (shared_ptr<Formula> or_operand: or_subf->getSubformulas()){
      auxilary_cnf_formulas.push_back(DepthReduce(Diamond::create(dia_f->getModality(), 1, or_operand)));
    }

    formula_set mainClause;
    CNF_form auxClauses;

    for (CNF_form& aux_cnf: auxilary_cnf_formulas){
      if (aux_cnf.size() == 1) {
        mainClause.insert(aux_cnf[0].begin(), aux_cnf[0].end());
        continue;
      }

      shared_ptr<Formula> z = cache->createVariable();

      mainClause.insert(Not::create(z)->negatedNormalForm());

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
        CNF_form cnf_subformula = DepthReduce(subf);
        modal_subf_cnfs.insert(modal_subf_cnfs.end(), cnf_subformula.begin(), cnf_subformula.end());
      } break;
      
      default:
        prop_subf.insert(subf);
        break;
      }
    }
    if (prop_subf.empty()) return modal_subf_cnfs;
    if (prop_subf.size() == 1){
      CNF_form cnf_subformula = DepthReduce(Diamond::create(dia_f->getModality(), 1, *(prop_subf.begin())));
      modal_subf_cnfs.insert(modal_subf_cnfs.end(), cnf_subformula.begin(), cnf_subformula.end());
      return modal_subf_cnfs;
    }

    CNF_form cnf_subf = convertToCNF(And::create(prop_subf));
    shared_ptr<Formula> z = cache->createVariable();

    for (formula_set& subf_clause: cnf_subf){
      subf_clause.insert(Not::create(z));
    }
    modal_subf_cnfs.push_back({Diamond::create(dia_f->getModality(), 1, z)});
    CNF_form auxilary_box = DepthReduceBoxFromCNF(cnf_subf, dia_f->getModality());
    
    modal_subf_cnfs.insert(modal_subf_cnfs.end(), auxilary_box.begin(), auxilary_box.end());
    return modal_subf_cnfs;
  } break;
  
  default:
    return {{Diamond::create(dia_f->getModality(), 1, dia_f->getSubformula())}};
    break;
  }

  CNF_form cnf_subf = convertToCNF(dia_f->getSubformula());

  shared_ptr<Formula> z = cache->createVariable();

  for (formula_set& clause: cnf_subf){
    clause.insert(z);
  }

  CNF_form auxilary_box_cnf = DepthReduceBoxFromCNF(cnf_subf, dia_f->getModality());
  auxilary_box_cnf.push_back({Diamond::create(dia_f->getModality(), 1, Not::create(z)->negatedNormalForm())});

  return auxilary_box_cnf;
}

CNF_form TrieformProverS5::DepthReduceAnd(shared_ptr<Formula> inp_formula){
  And* and_f = dynamic_cast<And*>(inp_formula.get());

  CNF_form cnf_formula;
  for (shared_ptr<Formula> subformula: and_f->getSubformulas()){
    CNF_form cnf_subformula = DepthReduce(subformula);

    cnf_formula.insert(cnf_formula.end(), cnf_subformula.begin(), cnf_subformula.end());
  }
  return cnf_formula;
}

CNF_form TrieformProverS5::DepthReduceOr(shared_ptr<Formula> inp_formula){
  Or* or_f = dynamic_cast<Or*>(inp_formula.get());

  formula_set mainClause;
  CNF_form newClauses;

  for (shared_ptr<Formula> subformula: or_f->getSubformulas()){
    CNF_form cnf_subformula = DepthReduce(subformula);

    if (cnf_subformula.size() == 1) {
      mainClause.insert(cnf_subformula[0].begin(), cnf_subformula[0].end());
      continue;
    }
    shared_ptr<Formula> z = cache->createVariable();

    mainClause.insert(Not::create(z)->negatedNormalForm());

    for (formula_set& clause: cnf_subformula){
      clause.insert(z);

      newClauses.push_back(clause);
    }
  }
  newClauses.push_back(mainClause);
  return newClauses;
}

CNF_form TrieformProverS5::DepthReduce(shared_ptr<Formula> inp_formula){
  switch (inp_formula->getType())
  {
  case FBox:
    return DepthReduceBox(inp_formula);
    break;

  case FDiamond:
    return DepthReduceDiamond(inp_formula);
    break;

  case FAnd:
    return DepthReduceAnd(inp_formula);
    break;

  case FOr:
    return DepthReduceOr(inp_formula);
    break;

  default:
    return {{inp_formula}};
    break;
  }
}

void TrieformProverS5::splitClause(formula_set clause, formula_set& modal_lits, formula_set& prop_lits){
  for (shared_ptr<Formula> S5_lit: clause){
    if (isPropLiteral(S5_lit)){
      prop_lits.insert(S5_lit);
    }
    else if (S5_lit->getType() == FOr){
      Or* o = dynamic_cast<Or*>(S5_lit.get());
      splitClause(o->getSubformulas(), modal_lits, prop_lits);
    }
    else {
      modal_lits.insert(S5_lit);
    }
  }
}

void TrieformProverS5::propagateOneClause(formula_set clause){
  if (clause.size() == 0) return;

  formula_set modal_lits;
  formula_set prop_lits;

  splitClause(clause, modal_lits, prop_lits);

  if (modal_lits.size() == 0){
    clauses.addClause(Or::create(clause));
    return;
  }

  if (modal_lits.size() == 1 && prop_lits.size() == 0){
    shared_ptr<Formula> modal_lit = *(modal_lits.begin());

    if (modal_lit->getType() == FBox){
      Box* box_formula = dynamic_cast<Box*>(modal_lit.get());
      clauses.addClause(box_formula->getSubformula());
      shared_ptr<Trieform> subtrie = getSubtrieOrEmpty(box_formula->getModality());

      subtrie->clauses.addClause(box_formula->getSubformula());
    }

    else {
      Diamond* dia_formula = dynamic_cast<Diamond*>(modal_lit.get());

      ModalClause modal_clause = {dia_formula->getModality(), True::create(), dia_formula->getSubformula()};
      clauses.addDiamondClause(modal_clause);
      ensureSubtrieExistence(dia_formula->getModality());
    }
    return;
  }

  
  if (modal_lits.size() == 1 && prop_lits.size() == 1){
    shared_ptr<Formula> modal_lit = *(modal_lits.begin());
    shared_ptr<Formula> prop_lit = *(prop_lits.begin());

    if (modal_lit->getType() == FBox){
      Box* box_formula = dynamic_cast<Box*>(modal_lit.get());
      int modality = box_formula->getModality();
      shared_ptr<Formula> right = box_formula->getSubformula();
      
      if (isPropLiteral(right)){
        shared_ptr<Formula> left = prop_lit->negate();
        ModalClause modal_clause = {modality, left, right};
        clauses.addBoxClause(modal_clause);
        clauses.addClause(Or::create({prop_lit, right}));
        ensureSubtrieExistence(modality);
        return;
      }

      shared_ptr<Formula> name = cache->getVariableOrCreate(modal_lit);
      Or* or_subf = dynamic_cast<Or*>(right.get());

      formula_set augmented_set = or_subf->getSubformulas();
      augmented_set.insert(name->negate());

      clauses.addClause(Or::create({prop_lit, name}));
      clauses.addBoxClause({modality, name, name});

      shared_ptr<Trieform> subtrie = getSubtrieOrEmpty(modality);
      subtrie->clauses.addClause(Or::create(augmented_set));
    }
    
    else{
      Diamond* diamond_formula = dynamic_cast<Diamond*>(modal_lit.get());
      int modality = diamond_formula->getModality();
      shared_ptr<Formula> right = diamond_formula->getSubformula();

      ModalClause modal_clause = {modality, prop_lit, right};
      clauses.addDiamondClause(modal_clause);
      ensureSubtrieExistence(modality);
    }
  }

  else {
    for (shared_ptr<Formula> modal_lit: modal_lits){
      if (modal_lit->getType() == FDiamond){
        Diamond* diamond_lit = dynamic_cast<Diamond*>(modal_lit.get());

        shared_ptr<Formula> name = cache->getVariableOrCreate(modal_lit);
        prop_lits.insert(name);

        ModalClause modal_clause = {diamond_lit->getModality(), name, diamond_lit->getSubformula()};
        clauses.addDiamondClause(modal_clause);
        ensureSubtrieExistence(diamond_lit->getModality());
        continue;
      }
      
      Box* box_formula = dynamic_cast<Box*>(modal_lit.get());
      shared_ptr<Formula> name = cache->getVariableOrCreate(modal_lit);

      prop_lits.insert(name);

      if (box_formula->getSubformula()->getType() == FOr) {
        Or* or_clause = dynamic_cast<Or*>(box_formula->getSubformula().get());
        
        formula_set augmented_clause = or_clause->getSubformulas();
        augmented_clause.insert(Not::create(name)->negatedNormalForm());

        shared_ptr<Trieform> subtrie = getSubtrieOrEmpty(box_formula->getModality());

        clauses.addBoxClause({box_formula->getModality(), name, name});
        subtrie->clauses.addClause(Or::create(augmented_clause));
      }
      else {
        ModalClause modal_clause = {box_formula->getModality(), name, box_formula->getSubformula()};
        clauses.addBoxClause(modal_clause);
        ensureSubtrieExistence(box_formula->getModality());
      }
    }

    clauses.addClause(Or::create(prop_lits));
  }
}

bool is_prop_lit(shared_ptr<Formula> formula){
  switch (formula->getType())
  {
  case FAtom:
  case FFalse:
  case FTrue:
    return true;
    break;

  case FNot:{
    Not* not_formula = dynamic_cast<Not*>(formula.get());

    switch (not_formula->getSubformula()->getType())
    {
      case FAtom:
      case FFalse:
      case FTrue:
        return true;
        break;

      default:
        return false;
        break;
    }
  }
  
  default:
    return false;
    break;
  }
}

bool is_correct_d1(CNF_form cnf_formula){
  for (formula_set clause: cnf_formula){
    for (shared_ptr<Formula> lit: clause){
      if (is_prop_lit(lit)) continue;

      if (lit->getType() == FDiamond){
        Diamond* dia_lit = dynamic_cast<Diamond*>(lit.get());
        if (!is_prop_lit(dia_lit->getSubformula())) return false;
        else continue;
      }

      if (lit->getType() != FBox){
        return false;
      }

      Box* box_lit = dynamic_cast<Box*>(lit.get());
      if (is_prop_lit(box_lit->getSubformula())) continue;

      if (box_lit->getSubformula()->getType() != FOr) return false;
      Or* or_subf = dynamic_cast<Or*>(box_lit->getSubformula().get());

      for (shared_ptr<Formula> or_lit: or_subf->getSubformulas()){
        if (!is_prop_lit(or_lit)) return false;
      }
    }
  }

  return true;
}

void TrieformProverS5::propagateClauses(const shared_ptr<Formula>& formula){
  CNF_form cnf_formula = DepthReduce(formula);
  
  for (formula_set clause: cnf_formula){
    propagateOneClause(clause);
  }
}
