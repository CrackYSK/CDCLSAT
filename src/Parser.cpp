#include "Parser.h"

int Parser::skipSpaces(std::istream &in) {
  int c;
  while ((c = in.get()) == ' ' || c == '\t' || c == '\n');
  return c;
}

void Parser::skipRestOfLine(std::istream &in) {
  while (in.get() != '\n');
}

bool Parser::readDIMACS(Formula &f, unsigned &num_of_vars, std::istream &in) {
  unsigned num_of_clauses;
  int c;

  while ((c = skipSpaces(in)) == 'c')
    skipRestOfLine(in);

  if (c != 'p') {
    return false;
  } else {
    std::string s;
    in >> s;
    if (s != "cnf") {
      return false;
    }

    in >> num_of_vars;
    in >> num_of_clauses;
  }

  for (unsigned i = 0; i < num_of_clauses; i++) {
    Clause c;
    int n;
    in >> n;

    while (!in.eof() && !in.fail() && n != 0) {
      c.insert(litFromInt(n));
      in >> n;
    }

    if (in.eof() || in.fail()) {
      return false;
    }

    f.insert(c);
  }
  return true;
}
