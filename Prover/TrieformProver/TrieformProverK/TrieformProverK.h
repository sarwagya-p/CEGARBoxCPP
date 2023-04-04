#ifndef TRIEFORM_PROVER_K
#define TRIEFORM_PROVER_K

#include "../../../Bitset/Bitset.h"
#include "../../../Clausifier/Trieform/Trieform.h"
#include "../../../Clausifier/TrieformFactory/TrieformFactory.h"
#include "../../LocalSolutionMemo/LocalSolutionMemo.h"
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

class TrieformProverK : public Trieform {
protected:
  unsigned int assumptionsSize = 0;
  LocalSolutionMemo localMemo;
  unordered_map<string, unsigned int> idMap;

  shared_ptr<Bitset> convertAssumptionsToBitset(literal_set literals);
  void updateSolutionMemo(const shared_ptr<Bitset> &assumptions,
                          Solution solution);

    void boxClausesT();
    void modalContextsT();

public:
  TrieformProverK();
  ~TrieformProverK();

  vector<literal_set> allConflicts;

  Solution prove(literal_set assumptions = literal_set());
  Solution prove(int depth, literal_set assumptions = literal_set());
  virtual void preprocess();
  virtual void prepareSAT(name_set extra = name_set());
  virtual shared_ptr<Bitset> fleshedOutAssumptionBitset(literal_set model);

  virtual shared_ptr<Trieform> create(const shared_ptr<Formula> &formula);
  virtual shared_ptr<Trieform> create(const shared_ptr<Formula> &formula,
                                      const vector<int> &newModality);
  virtual shared_ptr<Trieform> create(const vector<int> &newModality);
  bool clauseConflictsWithModel(literal_set clause, literal_set model);

  void localReductionT();
  void localReductionD();
  void localReduction5();

    shared_ptr<modal_clause_set> getAllBoxClauses5();

    void globallyAddBoxClauses(shared_ptr<modal_clause_set> boxClauses);
};

#endif
