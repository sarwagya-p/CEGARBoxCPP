#include "ProbationSolutionMemo.h"

ProbationSolutionMemo::ProbationSolutionMemo() {}
ProbationSolutionMemo::~ProbationSolutionMemo() {}

ProbationSolutionMemoResult
ProbationSolutionMemo::getFromMemo(const shared_ptr<vector<int>> &modalContext, const shared_ptr<Bitset> &assumptions) const {
    for (auto satisfiable : satSols) {
        if (satisfiable.first == modalContext) {
            if (satisfiable.second->contains(*assumptions)) {
                return {true, {true, literal_set()}};
            }
        }
    }
    return {false, {false, literal_set()}};
}

void ProbationSolutionMemo::insertSat(const shared_ptr<vector<int>> &modalContext, const shared_ptr<Bitset> &assumptions) {
    satSols.push_back({modalContext, assumptions});
}

void ProbationSolutionMemo::clear() {
    satSols.clear();
}
void ProbationSolutionMemo::resize(int n) {
    satSols.resize(n);
}
int ProbationSolutionMemo::size() {
    return satSols.size();
}

void ProbationSolutionMemo::updateMinimalRoot(int newMinimalRoot) {
    if (minimalRoot == -1) minimalRoot = newMinimalRoot;  
    else minimalRoot = min(minimalRoot, newMinimalRoot);
}

ProbationSolutionMemoState ProbationSolutionMemo::getState() {
    return {minimalRoot, satSols.size()};
}

void ProbationSolutionMemo::setState(ProbationSolutionMemoState pastInfo) {
    minimalRoot = pastInfo.first;
    satSols.resize(pastInfo.second);
}

 vector<pair<shared_ptr<vector<int>>, shared_ptr<Bitset>>> ProbationSolutionMemo::getSatSols() const {
    return satSols;
}
