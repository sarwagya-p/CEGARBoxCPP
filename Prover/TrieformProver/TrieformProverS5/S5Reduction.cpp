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

bool TrieformProverS5::isModalUnit(shared_ptr<Formula> formula){
  // A modal unit is either []phi, <>phi or a literal
  switch (formula->getType()){
    case FDiamond:
    case FBox:
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


CNF_form TrieformProverS5::convertToCNF(shared_ptr<Formula> d1_formula){
  switch (d1_formula->getType()){
  
  case FAnd: {
    CNF_form subformulas_cnf;

    And* and_f = dynamic_cast<And*>(d1_formula.get());

    for (shared_ptr<Formula> subformula: and_f->getSubformulas()){
      CNF_form subf_cnf = convertToCNF(subformula);
      subformulas_cnf.insert(subformulas_cnf.end(), subf_cnf.begin(), subf_cnf.end());
    }

    return subformulas_cnf;
  } break;

  case FOr: {
    /*
    phi = phi_1 | phi_2 | ... | phi_n
    Name each phi_i as z_i, make a names clause: (z_1 | z_2 ... |z_n)
    Make a main formula subformulas_cnf :
    For each phi_i cnf = C_i1 & C_i2 & ... & C_im - add C_i1 | ~z_i to subformulas_cnf
    */
    formula_set names_clause;
    CNF_form subformulas_cnf;

    Or* or_f = dynamic_cast<Or*>(d1_formula.get());
    
    
    for (shared_ptr<Formula> subformula: or_f->getSubformulas()){
      // In these two cases, no naming is needed
      if (isModalUnit(subformula)){
        names_clause.insert(subformula);
        continue;
      }

      CNF_form subf_cnf = convertToCNF(subformula);

      if (subf_cnf.size() == 1){
        subformulas_cnf.push_back(subf_cnf[0]);
        continue;
      }

      // Name the subformula
      shared_ptr<Formula> name = cache->createVariable();
      for (formula_set& clause: subf_cnf){
        clause.insert(name->negate());
        subformulas_cnf.push_back(clause);
      }
      names_clause.insert(name);
    }
    
    subformulas_cnf.push_back(names_clause);
    return subformulas_cnf;
  } break;

  default:
    // Since we assume nnf, the only case left is a S5literal
    return {{d1_formula}};
    break;
  }
}

void orCNFToCNF(vector<CNF_form>& cnfs, shared_ptr<Cache> cache, CNF_form total_formula){
  // Given a OR of CNFs (cnfs), convert it to CNF and add the formed clauses to total_formula
  formula_set mainClause;

  for (CNF_form cnf: cnfs){
    if (cnf.size() == 1) mainClause.insert(cnf[0].begin(),cnf[0].end());
    else {
      shared_ptr<Formula> z = cache->createVariable();
      mainClause.insert(z);

      for (formula_set clause: cnf){
        clause.insert(z->negate());
        total_formula.push_back(clause);
      }
    }
  }
  if (!mainClause.empty()) total_formula.push_back(mainClause);
}

CNF_form TrieformProverS5::DepthReduceBoxFromCNF(CNF_form subformula, int modality){
  CNF_form total_cnf_formula;

  // [](C1 & C2 ... & C3) == []C1 & []C2 & ... & []Cn
  for (formula_set subf_clause: subformula) {
    /*
    [](l1 | l2 | ... lk | (*)m1 | (*)m2 ... | (*)mr) == 
    [](l1 | l2 | ... lk) | (*)m1 | (*)m2 | ... | (*)mr

    Note that (*)mi need to be reduced recursively, and (may) need to be named
    
    auxilary_cnf_formulas is an OR of CNF formulas, which will be converted to CNF by naming, 
    and then added to total_cnf_formula
    */
    formula_set prop_clause;
    vector<CNF_form> auxilary_cnf_formulas;

    for (shared_ptr<Formula> unit: subf_clause){
      switch (unit->getType()){
      case FBox:
      case FDiamond:
        auxilary_cnf_formulas.push_back(DepthReduce(unit));
        break;

      default:
        prop_clause.insert(unit);
        break;
      }
    }

    if (!prop_clause.empty()) auxilary_cnf_formulas.push_back({{Box::create(modality, 1, Or::create(prop_clause))}});

    // If there is only one auxilary_cnf_formula, it is already in CNF form and can be added directly
    if (auxilary_cnf_formulas.size() == 1){
      total_cnf_formula.insert(total_cnf_formula.end(), auxilary_cnf_formulas[0].begin(), auxilary_cnf_formulas[0].end());
      continue;
    }
    
    // mainClause will contain the names of (*)mi and cnfs which are one clause only
    orCNFToCNF(auxilary_cnf_formulas, cache, total_cnf_formula);
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
  // <>(*)phi == (*)phi
  case FBox:
  case FDiamond:
    return DepthReduce(dia_f->getSubformula());

  // <>(phi_1 | phi_2 ... phi_n) == <>phi_1 | <>phi_2 | ... | <>phi_n
  // Note - naming required to convert to cnf
  case FOr: {
    Or* or_subf = dynamic_cast<Or*>(dia_f->getSubformula().get());
    vector<CNF_form> auxilary_cnf_formulas;

    for (shared_ptr<Formula> or_operand: or_subf->getSubformulas()){
      auxilary_cnf_formulas.push_back(DepthReduce(Diamond::create(dia_f->getModality(), 1, or_operand)));
    }

    CNF_form total_formula;
    orCNFToCNF(auxilary_cnf_formulas, cache, total_formula);
    return total_formula;
  } break;

  case FAnd: {
    And* and_f = dynamic_cast<And*>(dia_f->getSubformula().get());

    CNF_form modal_subf_cnfs;
    formula_set prop_subf;

    for (shared_ptr<Formula> subf: and_f->getSubformulas()){
      /*
      <>(phi_1 & phi_2 ... phi_k & (*)psi_1 & (*)psi_2 ... (*)psi_r) ==
      <>(phi_1 & phi_2 ... phi_k) & (*)psi_1 & (*)psi_2 & ... & (*)psi_r
      phi_i will be added to prop_subf, and (*)psi_j will be reduced recursively and added to modal_subf_cnfs
      */
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

    /*
    <>(C1 & C2 ... Cn) == <>z & [](z => C1 & z=> C2 & ... z=> Cn)
    */
    CNF_form cnf_subf = DepthReduce(And::create(prop_subf));
    shared_ptr<Formula> z = cache->createVariable();

    for (formula_set& subf_clause: cnf_subf){
      subf_clause.insert(z->negate());
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
  return {{Diamond::create(dia_f->getModality(), 1, dia_f->getSubformula())}};
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

  vector<CNF_form> cnf_subformulas;

  for (shared_ptr<Formula> subformula: or_f->getSubformulas()){
    CNF_form cnf_subformula = DepthReduce(subformula);
    cnf_subformulas.push_back(cnf_subformula);
  }

  CNF_form total_formula;
  orCNFToCNF(cnf_subformulas, cache, total_formula);
  return total_formula;
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
  // Given a clause (l1 | l2 ... lk | (*)m1 | (*)m2 ... | (*)mr), split it into modal_lits (mi) and prop_lits (li)
  for (shared_ptr<Formula> S5_lit: clause){
    if (isPropLiteral(S5_lit)){
      prop_lits.insert(S5_lit);
    }

    // This case should not occur, but is present to be safe
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
  // Need to convert a given form to mints normal form
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
    // Cases where clause is already in form a => (*)b
    shared_ptr<Formula> modal_lit = *(modal_lits.begin());
    shared_ptr<Formula> prop_lit = *(prop_lits.begin());

    if (modal_lit->getType() == FDiamond){
      // l1 | <>l2
      Diamond* diamond_formula = dynamic_cast<Diamond*>(modal_lit.get());
      int modality = diamond_formula->getModality();
      shared_ptr<Formula> right = diamond_formula->getSubformula();

      ModalClause modal_clause = {modality, prop_lit->negate(), right};
      clauses.addDiamondClause(modal_clause);
      ensureSubtrieExistence(modality);
    }
    else {
      Box* box_formula = dynamic_cast<Box*>(modal_lit.get());
      int modality = box_formula->getModality();
      shared_ptr<Formula> right = box_formula->getSubformula();
      
      if (isPropLiteral(right)){
        clauses.addBoxClause(modality, prop_lit->negate(), right);
        clauses.addClause(Or::create({prop_lit, right}));
        ensureSubtrieExistence(modality);
        return;
      }
      // // p | [](l1 | l2 ... ln) == p | []z & [](z => (l1 | l2 | ... ln))
      // shared_ptr<Formula> z = cache->createVariable();
      // clauses.addBoxClause(modality, prop_lit->negate(), z);
      // clauses.addClause(Or::create({prop_lit, z}));

      // shared_ptr<Trieform> subtrie = getSubtrieOrEmpty(modality);
      // Or* box_or_subf = dynamic_cast<Or*>(right.get());
      // formula_set augmented_clause = box_or_subf->getSubformulas();
      // augmented_clause.insert(z->negate());

      // shared_ptr<Formula> augmented_clause_formula = Or::create(augmented_clause);
      // subtrie->clauses.addClause(augmented_clause_formula);
      // clauses.addClause(augmented_clause_formula);
      // return;

      // Else if it is a box of or of literals, need naming - fall through
    }
  }

  // Cases where all modal literals need to be named
  // Any names for the modal literals will be added to prop_lits
  for (shared_ptr<Formula> modal_lit: modal_lits){
    /*
    For box or diamond of a literal: (*)l | C == z | C & z => (*)l 
    For box of or of literals: [](Cb) | C == z | C & z => []z & [](z => Cb)
    Note - need to handle reflexivity for boxes
    */
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
      augmented_clause.insert(name->negate());

      shared_ptr<Trieform> subtrie = getSubtrieOrEmpty(box_formula->getModality());

      clauses.addBoxClause({box_formula->getModality(), name, name});
      shared_ptr<Formula> augmented_formula = Or::create(augmented_clause);
      subtrie->clauses.addClause(augmented_formula);
      clauses.addClause(augmented_formula);      
    }
    else {
      ModalClause modal_clause = {box_formula->getModality(), name, box_formula->getSubformula()};
      clauses.addBoxClause(modal_clause);
      clauses.addClause(Or::create({name->negate(), box_formula->getSubformula()}));
      ensureSubtrieExistence(box_formula->getModality());
    }
  }

  clauses.addClause(Or::create(prop_lits));
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
