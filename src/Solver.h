#ifndef __SOLVER_H__
#define __SOLVER_H__ 1

#include <map>
#include "representation.h"
#include "Valuation.h"

class Solver {
public:
  Solver(const Formula &f, unsigned num_of_vars)
    : _formula(f), _valuation(num_of_vars),
    _initial_formula_size(_formula.size()),
    _cn(-1)
  {}

  bool checkConflict();
  bool checkUnit(Literal &l, Clause &c);
  bool chooseDecisionLiteral(Literal &l);
  bool canBackjump();

  void applyUnitPropagation(const Literal & l, const Clause & c);
  void applyDecide(const Literal & l);
  void applyBackjump(unsigned level);

  void initialAnalysis();
  void applyExplainUIP();
  void applyLearn();
  bool isUIP();
  void applyExplain(const Literal & l);
  int getBackjumpLevel();
  Clause resolve(const Clause & c1, const Clause & c2, const Literal & l);

  bool solve();
private:
  Formula _formula;
  Valuation _valuation;
  Clause _conflict;
  std::map<Literal, Clause> _reason;
  int _initial_formula_size;
  int _cn; // number of literals from conflict clause on last decision level
};

#endif