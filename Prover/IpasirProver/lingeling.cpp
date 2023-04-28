// A wrapper for Lingeling for convenient use
// TODO destructors
#include "lingeling.h"

using namespace std;

// constructors
Lingeling::Lingeling() {
  solver = ipasir_init();
}


Lingeling::~Lingeling() {
  // TODO destructors
  ipasir_release(solver);
}

void Lingeling::flush_cache() {
  assert(0); // not implemented
}

Lingeling Lingeling::clone() {
  assert(0); // TODO causes problems.. look into it
  //return Lingeling(lglclone(solver));
  return Lingeling();
}


void Lingeling::add_clause(const vector<int>& inClause) {
  for (int i=0; i<inClause.size(); i++) {
    ipasir_add(solver, inClause[i]);
    max_var = max(max_var, abs(inClause[i]));
  }
  ipasir_add(solver, 0);
}

void Lingeling::add_clauses(const vector<vector<int>>& inClauses) {
  for (int j=0; j<inClauses.size(); j++){
    for (int i=0; i<inClauses[j].size(); i++) {
      ipasir_add(solver, inClauses[j][i]);
      max_var = max(max_var, abs(inClauses[j][i]));
    }
    ipasir_add(solver, 0);
  }
}

int Lingeling::create_variable() {
    max_var += 1;
    return max_var;
}
bool Lingeling::solve(const vector<int>& assumptions) {
  last_assumptions = assumptions;
  for (int i=0; i<assumptions.size(); i++){
      max_var = max(max_var, abs(assumptions[i]));
    ipasir_assume(solver, assumptions[i]);
  }
  int rawOutput = ipasir_solve(solver);
  if (rawOutput == 10) return true;
  if (rawOutput == 20) return false;
  assert(0);
}

// have a second set of assumptions which are not mentioned in "used_assumptions" 
bool Lingeling::solve_unmentioned_assumptions(const vector<int>& assumptions, const vector<int>& unmentioned_assumptions) {
  last_unmentioned_assumptions = unmentioned_assumptions;
  last_assumptions = assumptions;
  for (int i=0; i<assumptions.size(); i++){
    ipasir_assume(solver, assumptions[i]);
  }
  for (int i=0; i<unmentioned_assumptions.size(); i++){
    ipasir_assume(solver, unmentioned_assumptions[i]);
  }
  //cout << "start actual solving" << endl;
  int rawOutput = ipasir_solve(solver);
  //cout << "end actual solving" << endl;
  if (rawOutput == 10) return true;
  if (rawOutput == 20) return false;
  assert(0);
}


vector<int> Lingeling::get_model(int lowest, int largest) {
  // include the smallest and largest variable wanted
  vector<int> assignments;
  for(int i=lowest; i<=largest; i++){
    const int val = ipasir_val(solver, i);
    if (val) assignments.push_back(val);
    else     assignments.push_back(i); // as the value is not important, choose positive
  }
  return assignments;
}

vector<int> Lingeling::get_model() {
  vector<int> assignments(max_var);
  for(int i=1; i<=max_var; i++){
    const int val = ipasir_val(solver, i);
    if (abs(val) != i) 
        assignments[i-1] = i;
    else if (val) {
      assignments[i-1] = val;
    }
    else     {
      assignments[i-1] = i; // Not given a value: as the value is not important, choose positive 
    }
  }
  return assignments;
}

vector<int> Lingeling::used_assumptions() {
  vector<int> return_assumptions;
  for (int i=0; i < last_assumptions.size(); i++){
    if (ipasir_failed(solver, last_assumptions[i])){
      return_assumptions.push_back(last_assumptions[i]);
    }
  }
  return return_assumptions;
}

void Lingeling::set_important(const vector<int>& variables) {
  assert(0); // not implemented
}
