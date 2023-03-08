#ifndef TRIEFORM_PROVER_Kt
#define TRIEFORM_PROVER_Kt

#include "../../../Bitset/Bitset.h"
#include "../../../Clausifier/Trieform/Trieform.h"
#include "../../../Clausifier/TrieformFactory/TrieformFactory.h"
#include "../../LocalSolutionMemo/LocalSolutionMemo.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <map>

using namespace std;

class TrieformProverKt : public Trieform {
protected:
  int numRelations = -1;
  unsigned int assumptionsSize = 0;
  LocalSolutionMemo localMemo;
  unordered_map<string, unsigned int> idMap;
  shared_ptr<Bitset> convertAssumptionsToBitset(literal_set literals);
  void updateSolutionMemo(const shared_ptr<Bitset> &assumptions,
                          Solution solution);
  
bool isInHistory(vector<shared_ptr<Bitset>> history, shared_ptr<Bitset> bitset);
vector<shared_ptr<Bitset>> history;
public:
  TrieformProverKt();
  ~TrieformProverKt();
  
  static map<vector<int>, shared_ptr<TrieformProverKt>> all_trieforms;
  virtual shared_ptr<TrieformProverKt> convertToGrid(vector<int>& modal_context);
  Solution prove(literal_set assumptions = literal_set());
  virtual void getStats();
  virtual void preprocess();
  static void doResiduation();
  virtual void buildConnections();
  virtual void removeTrueAndFalse();
  virtual void prepareSAT(name_set extra = name_set());
  virtual bool isSatisfiable();

  virtual shared_ptr<Trieform> create(const shared_ptr<Formula> &formula);
  virtual shared_ptr<Trieform> create(const shared_ptr<Formula> &formula,
                                      const vector<int> &newModality);
  virtual shared_ptr<Trieform> create(const vector<int> &newModality);
  string toString();

  shared_ptr<TrieformProverKt> createGridTrie();
};

#endif
