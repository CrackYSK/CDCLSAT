#include "Valuation.h"

unsigned Valuation::current_level() const {
  return _curr_level;
}

void Valuation::push(Literal l, bool decide) {
  if (decide) {
    _curr_level++;
  }

  _stack.push_back(std::make_pair(l, _curr_level));
  _values[varFromLit(l)] = isPositive(l) ? B_TRUE : B_FALSE;
}

Literal Valuation::backjumpToLevel(unsigned level) {
  Literal l;
  while (_stack.back().second > level)  {
    _values[varFromLit(_stack.back().first)] = B_UNDEFINED;
    l = _stack.back().first;
    _stack.pop_back();
  }
  _curr_level = level;

  return l;
}

bool Valuation::findFirstUndefined(Variable &v) const {
  for (unsigned i = 0; i < _values.size(); i++) {
    if (_values[i] == B_UNDEFINED) {
      v = i ;
      return true;
    }
  }

  return false;
}

ExtendedBoolean Valuation::variableValue(Variable v) const {
  return _values[v];
}

ExtendedBoolean Valuation::literalValue(Literal l) const {
  return isPositive(l) ? _values[varFromLit(l)] : !_values[varFromLit(l)];
}

bool Valuation::isClauseFalse(const Clause &c) const {
  for (auto i = c.begin(); i != c.end(); i++) {
    if (literalValue(*i) == B_TRUE || literalValue(*i) == B_UNDEFINED) {
      return false;
    }
  }

  return true;
}

bool Valuation::isClauseUnit(const Clause &c, Literal &l) const {
  bool found = false;

  for (auto i = c.begin(); i != c.end(); i++) {
    if (literalValue(*i) == B_TRUE) {
      return false;
    }

    if (literalValue(*i) == B_UNDEFINED) {
      if (!found) {
        found = true;
        l = *i;
      } else {
        return false;
      }
    }
  }

  return found;
}

void Valuation::printValuation(std::ostream &out) const {
  for (unsigned i = 0; i < _values.size(); i++) {
    if (_values[i] == B_UNDEFINED) {
      out << "U ";
    } else if (_values[i] == B_TRUE) {
      out << static_cast<int>(i + 1) << " ";
    } else {
      out << -static_cast<int>(i + 1) << " ";
    }
  }

  out << 0 << std::endl;
}

void Valuation::printStack(std::ostream &out) const {
  unsigned level = 0;
  for (unsigned i = 0; i < _stack.size(); i++) {
    if (_stack[i].second > level) {
      out << "| ";
      level++;
    }
    out << intFromLit(_stack[i].first) << " ";
  }

  out << 0 << std::endl;
}
