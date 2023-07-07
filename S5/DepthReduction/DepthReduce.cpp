#include "DepthReduce.hpp"
#include "../S5cnf/S5cnf.hpp"

shared_ptr<Formula> depthReduce(shared_ptr<Formula> nnf_formula){
    switch (nnf_formula->getType())

    case FAtom:
    case FFalse:
    case FTrue:
    case FNot:

    case FAnd:
    case FOr:

    case FBox:
    case FDiamond:
};