//
// Created by Mohamed El Zarei on 27/12/2021.
//

#ifndef ZREGEXSTANDALONE_CPP_H
#define ZREGEXSTANDALONE_CPP_H

#include <fstream>
#include <string>

#include "codegen.h"
#include "fa/fa.h"
#include "spdlog/fmt/fmt.h"

namespace ZRegex {
  class CppCodeGen {
  public:
    static void Generate(std::unique_ptr<FiniteAutomaton> dfa, const char* filename,
                         const Encoding& encoding);

  private:
    static void GenerateUtf8(std::ofstream& fs);
    static void GenerateDebuggingMain(std::ofstream& fs);
    static void GenerateTraverse(std::unique_ptr<FiniteAutomaton> dfa, std::ofstream& fs,
                                 const Encoding& encoding);
    static void GenerateState(const FiniteAutomatonState& state, std::ofstream& fs,
                              const Encoding& encoding);
  };

}  // namespace ZRegex

#endif  // ZREGEXSTANDALONE_CPP_H
