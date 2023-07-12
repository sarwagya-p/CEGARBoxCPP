#ifndef S5TOKT
#define S5TOKT

#include "./DepthReduction/S5Formula.h"

shared_ptr<Formula> reduceS5ToKT(shared_ptr<Formula> inp_formula);

#endif