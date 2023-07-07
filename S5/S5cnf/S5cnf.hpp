#ifndef S5CNF
#define S5CNF

#include "../../Formula/And/And.h"
#include "../../Formula/Atom/Atom.h"
#include "../../Formula/Box/Box.h"
#include "../../Formula/Diamond/Diamond.h"
#include "../../Formula/FEnum/FEnum.h"
#include "../../Formula/Formula/Formula.h"
#include "../../Formula/Or/Or.h"

bool isLiteral(shared_ptr<Formula> formula);

// While converting to cnf, a literal is stored as shared_ptr<Formula>
// Clauses are stored as formula_set, and total formula is a vector of clauses
typedef vector<formula_set> CNF_form;

CNF_form convert_CNF(shared_ptr<Formula> d1_formula);

formula_set CNF_to_formula_clauses(CNF_form cnf_formula);

#endif