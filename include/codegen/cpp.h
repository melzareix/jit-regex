//
// Created by Mohamed El Zarei on 27/12/2021.
//

#ifndef ZREGEXSTANDALONE_CPP_H
#define ZREGEXSTANDALONE_CPP_H

#include <fstream>
#include <string>
#include "spdlog/fmt/fmt.h"

#include "../automaton/Automaton.h"

namespace ZRegex {
  class CppCodeGen {
  public:
    static void Generate(Automaton* dfa, const char* filename);

  private:
    static void GenerateTraverse(Automaton* dfa, std::ofstream& fs);
    static void GenerateState(const State& state,std::ofstream& fs, const std::vector<uint32_t> &accept_states);
  };

}  // namespace ZRegex

#endif  // ZREGEXSTANDALONE_CPP_H
