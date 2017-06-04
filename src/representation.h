#ifndef __REPRESENTATION_H__
#define __REPRESENTATION_H__ 1

#include <iostream>
#include <set>

typedef unsigned Variable;
typedef unsigned Literal;

enum Polarity {P_POSITIVE, P_NEGATIVE};

Literal litFromVar(Variable v, Polarity p);
Variable varFromLit(Literal l);

bool isPositive(Literal l);
bool isNegative(Literal l);

Literal oppositeLiteral(Literal l);

int intFromLit(Literal l);
Literal litFromInt(int i);

typedef std::set<Literal> Clause;
typedef std::set<Clause> Formula;

void printClause(const Clause &c, std::ostream &out);

enum ExtendedBoolean { B_TRUE, B_FALSE, B_UNDEFINED };

ExtendedBoolean operator!(ExtendedBoolean b);

#endif
