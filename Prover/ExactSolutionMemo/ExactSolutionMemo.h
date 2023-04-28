#ifndef EXACT_SOLUTION_MEMO_H
#define EXACT_SOLUTION_MEMO_H

#include "../../Bitset/Bitset.h"
#include "../Literal/Literal.h"
#include "../ProbationSolutionMemo/ProbationSolutionMemo.h"
#include "../LocalSolutionMemo/LocalSolutionMemo.h"
#include <memory>
#include <vector>

using namespace std;

struct LiteralSetHash {
    size_t operator()(const literal_set& assumptions) const {
        int hash = 0;
        for (auto x : assumptions) hash += x.hash();
        return hash;
    }
};

struct LiteralSetEqual {
    bool operator()(const literal_set& assumptions1, const literal_set& assumptions2) const {
        if (assumptions1.size() != assumptions2.size()) return false;
        for (auto x : assumptions1) {
            if (assumptions2.find(x) == assumptions2.end()) {
                return false;
            }
        }
        return true;
    }
};

class ExactSolutionMemo {
private:
    unordered_set<literal_set, LiteralSetHash, LiteralSetEqual> satSols;
  unordered_map<literal_set, literal_set, LiteralSetHash, LiteralSetEqual> unsatSols;

public:
  ExactSolutionMemo();
  ~ExactSolutionMemo();

  LocalSolutionMemoResult
  getFromMemo(const literal_set &assumptions) const;

  void insertSat(const literal_set &assumptions);
  void insertUnsat(const literal_set &assumptions,
                   const literal_set &unsatCore);
    
};

#endif
