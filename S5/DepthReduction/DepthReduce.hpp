#ifndef DEPTHREDUCE
#define DEPTHREDUCE

#include "../../Formula/And/And.h"
#include "../../Formula/Atom/Atom.h"
#include "../../Formula/Box/Box.h"
#include "../../Formula/Diamond/Diamond.h"
#include "../../Formula/FEnum/FEnum.h"
#include "../../Formula/Formula/Formula.h"
#include "../../Formula/Or/Or.h"

shared_ptr<Formula> S5CNF(shared_ptr<Formula> nnf_formula);
shared_ptr<Formula> depthReduce(shared_ptr<Formula> nnf_formula);

#endif