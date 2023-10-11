#ifndef TRIEFORM_PROVER_S5
#define TRIEFORM_PROVER_S5

#include "../../../Bitset/Bitset.h"
#include "../../../Clausifier/Trieform/Trieform.h"
#include "../../../Clausifier/TrieformFactory/TrieformFactory.h"
#include "../../GlobalSolutionMemo/GlobalSolutionMemo.h"
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

typedef vector<formula_set> CNF_form;

class TrieformProverS5 : public Trieform {
private:
  bool isPropLiteral(shared_ptr<Formula> inp_formula);
  bool isS5Literal(shared_ptr<Formula> inp_formula);

  CNF_form convertToCNF(shared_ptr<Formula> inp_formula);

  CNF_form DepthReduceBoxFromCNF(CNF_form cnf_subf, int modality);
  CNF_form DepthReduceBox(shared_ptr<Formula> inp_formula);
  CNF_form DepthReduceDiamond(shared_ptr<Formula> inp_formula);
  CNF_form DepthReduceAnd(shared_ptr<Formula> inp_formula);
  CNF_form DepthReduceOr(shared_ptr<Formula> inp_formula);

  CNF_form DepthReduce(shared_ptr<Formula> inp_formula);

  string cnfToString(CNF_form cnf_formula);

  void splitClause(formula_set clause, formula_set& modal_lits, formula_set& prop_lits);
  void propagateOneClause(formula_set clause);

  
protected:
  static unsigned int assumptionsSize;
  static unordered_map<string, unsigned int> idMap;
  
  shared_ptr<Bitset> convertAssumptionsToBitset(literal_set literals);

  void reflexiveHandleBoxClauses();
  virtual void propagateClauses(const shared_ptr<Formula> &formula);

public:
  TrieformProverS5();
  ~TrieformProverS5();

  virtual Solution prove(literal_set assumptions);
  virtual void preprocess();
  virtual void prepareSAT(name_set extra = name_set());

  virtual shared_ptr<Trieform> create(const shared_ptr<Formula> &formula);
  virtual shared_ptr<Trieform> create(const shared_ptr<Formula> &formula,
                                      const vector<int> &newModality);
  virtual shared_ptr<Trieform> create(const vector<int> &newModality);
};

#endif
