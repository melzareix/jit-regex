#include <parser/AutomataVisitor.h>
#include <parser/RegExp.h>
#include <zregex/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>

using namespace std;
auto main(int argc, char** argv) -> int {
  std::string s(argv[1]);
  RegExp r(s);
  auto node = r.parse();

  // evaluate ast and create the automata
  AutomataVisitor* v = new AutomataVisitor();
  node->accept(v);
  return 0;
}
