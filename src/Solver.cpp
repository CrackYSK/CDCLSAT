#include "Solver.h"

bool Solver::checkConflict(Clause &c) {
  for (auto it = _formula.begin(); it != _formula.end(); it++) {
    if (_valuation.isClauseFalse(*it)) {
      c = *it;
      return true;
    }
  }

  return false;
}

bool Solver::checkUnit(Literal &l) {
  for (auto it = _formula.begin(); it != _formula.end(); it++) {
    if (_valuation.isClauseUnit(*it, l)) {
      return true;
    }
  }

  return false;
}

bool Solver::chooseDecisionLiteral(Literal &l) {
  Variable v;
  if (!_valuation.findFirstUndefined(v)) {
    return false;
  } else {
    l = litFromVar(v, P_POSITIVE);
    return true;
  }
}

bool Solver::canBackjump() {
  return _valuation.current_level() > 0;
}

void Solver::applyUnitPropagation(Literal l) {
  _valuation.push(l);
}

void Solver::applyDecide(Literal l) {
  _valuation.push(l, true);
}

void Solver::applyBackjump(unsigned level) {
  Literal l = _valuation.backtrackToLevel(level);
  _valuation.push(l);
}

/*
Check http://poincare.math.rs/~filip/phd/sat-tutorial.pdf
for additional information (pages 18, 19 and 20)
*/
bool Solver::solve() {
  Literal l;
  Clause c; // conflict clause

  while (true) {
    if (checkConflict(c)) {
      // TODO: analyze conflict and learn from it

      if (canBackjump()) {
        // TODO: after conflict is analyzed backtrack to desired level
      } else {
        // TODO: empty conflict clause
        return false;
      }
    } else if (checkUnit(l)) {
      applyUnitPropagation(l);
    } else if (chooseDecisionLiteral(l)) {
      applyDecide(l);
    } else {
      return true;
    }
  }
}