#ifndef __SOLVER_H__
#define __SOLVER_H__ 1

#include "representation.h"
#include "Valuation.h"

class Solver {
public:
  Solver(const Formula &f, unsigned num_of_vars)
    : _formula(f), _valuation(num_of_vars)
  {
    _initial_formula_size = _formula.size();
  }

  bool checkConflict(Clause &c);
  bool checkUnit(Literal &l);
  bool chooseDecisionLiteral(Literal &l);
  bool canBackjump();

  void applyUnitPropagation(Literal l);
  void applyDecide(Literal l);
  void applyBackjump(unsigned level);

  bool solve();
private:
  Formula _formula;
  Valuation _valuation;
  int _initial_formula_size;
};

#endif