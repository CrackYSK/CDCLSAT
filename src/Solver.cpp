#include "Solver.h"

/*
Check http://poincare.math.rs/~filip/phd/sat-tutorial.pdf
for additional information (pages 18, 19 and 20)
*/

bool Solver::checkConflict() {
  for (auto it = _formula.begin(); it != _formula.end(); it++) {
    if (_valuation.isClauseFalse(*it)) {
      _conflict = *it;
      return true;
    }
  }

  return false;
}

bool Solver::checkUnit(Literal &l, Clause &c) {
  for (auto it = _formula.begin(); it != _formula.end(); it++) {
    if (_valuation.isClauseUnit(*it, l)) {
      c = *it;
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

void Solver::applyUnitPropagation(const Literal & l, const Clause & c) {
  _valuation.push(l);
  _reason[l] = c;
}

void Solver::applyDecide(const Literal & l) {
  _valuation.push(l, true);
}

void Solver::applyBackjump(unsigned level) {
  Literal l = _valuation.lastAssertedLiteral(invertClause(_conflict));
  _valuation.backjumpToLevel(level);
  _valuation.push(oppositeLiteral(l));
  _reason[oppositeLiteral(l)] = _conflict;
}

void Solver::initialAnalysis() {
  _cn = _valuation.numberOfTopLevelLiterals(invertClause(_conflict));
}

void Solver::applyExplainUIP() {
  while (!isUIP()) {
    applyExplain(_valuation.lastAssertedLiteral(invertClause(_conflict)));
  }
}

void Solver::applyLearn() {
  _formula.insert(_conflict);
}

bool Solver::isUIP() {
  if (_cn > 1 || _cn == -1) {
    return false;
  } else {
    return true;
  }
}

void Solver::applyExplain(const Literal & l) {
  Clause reason = _reason[l];

  _conflict = resolve(_conflict, reason, l);

  _cn = _valuation.numberOfTopLevelLiterals(invertClause(_conflict));
}

Clause Solver::resolve(const Clause & c1, const Clause & c2, const Literal & l) {
  Clause resolvent;
  for (auto it = c1.begin(); it != c1.end(); it++) {
    if (oppositeLiteral(l) != *it) {
      resolvent.insert(*it);
    }
  }

  for (auto it = c2.begin(); it != c2.end(); it++) {
    if (l != *it) {
      resolvent.insert(*it);
    }
  }

  return resolvent;
}

int Solver::getBackjumpLevel() {
  Literal l = _valuation.lastAssertedLiteral(invertClause(_conflict));
  Clause temp;
  for (auto it = _conflict.begin(); it != _conflict.end(); it++) {
    if (*it != l) {
      temp.insert(*it);
    }
  }

  return _valuation.lastAssertedLiteralLevel(temp);
}

bool Solver::solve() {
  Literal l;
  Clause reason; // reason clause

  while (true) {
    if (checkConflict()) {
      initialAnalysis();

      if (canBackjump()) {
        applyExplainUIP();
        applyLearn();
        applyBackjump(getBackjumpLevel());
        _conflict.clear();
      } else {
        // TODO: empty conflict clause
        return false;
      }
    } else if (checkUnit(l, reason)) {
      applyUnitPropagation(l, reason);
    } else if (chooseDecisionLiteral(l)) {
      applyDecide(l);
    } else {
      return true;
    }
  }
}