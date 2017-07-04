#include "representation.h"

Literal litFromVar (Variable v, Polarity p) {
  return p == P_POSITIVE ? v << 1 :  (v << 1) | 1;
}

Variable varFromLit(Literal l) {
  return l >> 1;
}

bool isPositive(Literal l) {
  return !(l & 1);
}

bool isNegative(Literal l) {
  return l & 1;
}

Literal oppositeLiteral(Literal l) {
  return l ^ 1;
}

int intFromLit(Literal l) {
  return isPositive(l) ? (int) varFromLit(l) + 1 : -(int) (varFromLit(l) + 1);
}

Literal litFromInt(int i) {
  return i > 0 ? litFromVar(i - 1, P_POSITIVE) : litFromVar(-i - 1, P_NEGATIVE);
}

bool clauseContainsLiteral(const Clause & c, const Literal & l) {
  for (auto it = c.begin(); it != c.end(); it++) {
    if (*it == l) {
      return true;
    }
  }

  return false;
}

Clause invertClause(const Clause & c) {
  Clause new_clause;
  for (auto it = c.begin(); it != c.end(); it++) {
    new_clause.insert(oppositeLiteral(*it));
  }

  return new_clause;
}

void printClause(const Clause &c, std::ostream &out) {
  out << "[ ";
  for (auto i = c.cbegin(); i != c.cend(); i++) {
    out << intFromLit(*i) << " ";
  }
  out << "]";

  out << std::endl;
}

ExtendedBoolean operator!(ExtendedBoolean b) {
  if (b == B_TRUE) {
    return B_FALSE;
  } else if (b == B_FALSE) {
    return B_TRUE;
  } else {
    return B_UNDEFINED;
  }
}


void printFormula(const Formula & f, std::ostream & out) {
  out << "[" << std::endl;
  for (Clause c : f) {
    out << "\t";
    printClause(c, out);
  }
  out << "]" << std::endl;
}