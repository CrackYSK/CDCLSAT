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
#ifdef DEBUG
  std::cout << "Literal " << intFromLit(l) << " propagated because ";
  printClause(c, std::cout);
#endif
}

void Solver::applyDecide(const Literal & l) {
  _valuation.push(l, true);
#ifdef DEBUG
  std::cout << "Literal " << intFromLit(l) << " decided" << std::endl;
#endif
}

void Solver::applyBackjump(const Literal & l) {
  std::vector<Literal> literals;
  Literal lit_to_prop;
  bool empty;
  _valuation.lastAssertedLiteral(invertClause(_conflict), lit_to_prop, empty);
  _valuation.backjumpToLiteral(l, literals);
  for (Literal lit : literals) {
    _reason.erase(lit);
  }
  applyUnitPropagation(oppositeLiteral(lit_to_prop), _conflict);
}

void Solver::applyBackjumpToStart() {
  Literal lit_to_prop;
  bool empty;
  _valuation.lastAssertedLiteral(invertClause(_conflict), lit_to_prop, empty);
  _reason.clear();
  restart();
  applyUnitPropagation(oppositeLiteral(lit_to_prop), _conflict);
}

void Solver::initialAnalysis() {
  _cn = _valuation.numberOfTopLevelLiterals(invertClause(_conflict));
}

void Solver::applyExplainUIP() {
  while (!isUIP()) {
    Literal l;
    bool empty;
    _valuation.lastAssertedLiteral(invertClause(_conflict), l, empty);
    applyExplain(l);
  }
}

void Solver::applyExplainEmpty() {
  while (!_conflict.empty()) {
    Literal l;
    bool empty;
    _valuation.lastAssertedLiteral(invertClause(_conflict), l, empty);
    applyExplain(l);
  }
}

void Solver::applyLearn() {
  _formula.insert(_conflict);

#ifdef DEBUG
  std::cout << "Clause learned: ";
  printClause(_conflict, std::cout);
#endif
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

void Solver::getBackjumpLiteral(Literal &l, unsigned & level) {
  bool empty;
  _valuation.lastAssertedLiteral(invertClause(_conflict), l, empty);
  Clause temp;
  for (auto it = _conflict.begin(); it != _conflict.end(); it++) {
    if (*it != oppositeLiteral(l)) {
      temp.insert(*it);
    }
  }

  _valuation.lastAssertedLiteral(invertClause(temp), l, empty);

  if (empty) {
    level = 0;
  } 

}

void Solver::restart() {
  _valuation.clear();
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
        unsigned level;
        Literal backjump_literal;
        getBackjumpLiteral(backjump_literal, level);
        if (level != 0) {
          applyBackjump(backjump_literal);
        } else {
          applyBackjumpToStart();
        }
        _conflict.clear();
      } else {
        applyExplainEmpty();
        applyLearn();
        _status = B_FALSE;
        return false;
      }
    } else if (checkUnit(l, reason)) {
      applyUnitPropagation(l, reason);
    } else if (chooseDecisionLiteral(l)) {
      applyDecide(l);
    } else {
      _status = B_TRUE;
      return true;
    }
  }
}

void Solver::printModel(std::ostream & out) {
  if (_status == B_TRUE) {
    _valuation.printValuation(out);
  }
}