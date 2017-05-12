#ifndef __PARSER_H__
#define __PARSER_H__ 1

#include <iostream>
#include "representation.h"

class Parser {
public:
  bool readDIMACS(Formula &f, unsigned &num_of_vars, std::istream &in);
private:
  int skipSpaces(std::istream &in);
  void skipRestOfLine(std::istream &in);
};

#endif
