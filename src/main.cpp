#include <cstdlib>
#include <iostream>
#include <fstream>
#include "representation.h"
#include "Parser.h"
#include "Valuation.h"

int main(int argc, const char **argv) {
  if (argc != 2) {
    std::cerr << "You should specify input file as program argument!" 
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream in(argv[1]);

  if (!in.is_open()) {
    std::cerr << "Error with input file!" << std::endl;
    exit(EXIT_FAILURE);
  }

  Parser p;
  Formula f;
  unsigned num_of_vars;

  if (!p.readDIMACS(f, num_of_vars, in)) {
    std::cerr << "File not in DIMACS format!" << std::endl;
    exit(EXIT_FAILURE);
  }

#ifdef DEBUG
  std::cout << "Formula from readDIMACS:" << std::endl << "[" << std::endl;
  for (Clause c : f) {
    printClause(c, std::cout);
  }
  std::cout << "]" << std::endl;
#endif

  Valuation v(num_of_vars);

#ifdef DEBUG
  std::cout << std::endl << "Starting valuation:" << std::endl;
  v.printValuation(std::cout);
#endif

  return 0;
}