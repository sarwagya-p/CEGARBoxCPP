#ifndef PROBATION_SOLUTION_MEMO_H
#define PROBATION_SOLUTION_MEMO_H

#include "../../Bitset/Bitset.h"
#include "../Literal/Literal.h"
#include <memory>
#include <vector>
#include <map>

class TrieformProverKt;

using namespace std;

typedef pair<int, int> ProbationSolutionMemoState;

struct ProbationSolutionMemoResult {
    bool inSatMemo;
    Solution result;
};

class ProbationSolutionMemo {
    private:
        vector<pair<shared_ptr<vector<int>>, shared_ptr<Bitset>>> satSols;

    public:
        int minimalRoot = -1;

        ProbationSolutionMemo();
        ~ProbationSolutionMemo();

        ProbationSolutionMemoResult
            getFromMemo(const shared_ptr<vector<int>> &modalContext, const shared_ptr<Bitset> &assumptions) const;

        void insertSat(const shared_ptr<vector<int>> &modalContext, const shared_ptr<Bitset> &assumptions);
        void clear();
        void resize(int n);
        int size();
        
        void updateMinimalRoot(int newMinimalRoot);
        ProbationSolutionMemoState getState();
        void setState(ProbationSolutionMemoState pastInfo);
        
        vector<pair<shared_ptr<vector<int>>, shared_ptr<Bitset>>> getSatSols() const;
};

#endif
