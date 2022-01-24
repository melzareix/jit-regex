//
// Created by Mohamed El Zarei on 27/12/2021.
//

#ifndef ZREGEXSTANDALONE_CPP_H
#define ZREGEXSTANDALONE_CPP_H

#include <fstream>
#include <string>
#include "spdlog/fmt/fmt.h"
#include "fa/fa.h"


namespace ZRegex {
  class CppCodeGen {
  public:
    static void Generate(std::unique_ptr<FiniteAutomaton> dfa, const char* filename);

  private:
    static void GenerateTraverse(std::unique_ptr<FiniteAutomaton> dfa, std::ofstream& fs);
    static void GenerateState(const FiniteAutomatonState& state,std::ofstream& fs);
  };

}  // namespace ZRegex

#endif  // ZREGEXSTANDALONE_CPP_H
