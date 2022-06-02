//
// Created by Mohamed El Zarei on 27/12/2021.
//

#ifndef ZREGEXSTANDALONE_CPP_H
#define ZREGEXSTANDALONE_CPP_H

#include <fstream>
#include <string>

#include "codegen.h"
#include "fa/fa.h"
#include "opts.h"
#include "spdlog/fmt/fmt.h"

namespace ZRegex {
  class CppCodeGen {
  public:
    CppCodeGen(std::unique_ptr<FiniteAutomaton> dfa, const CodegenOpts& opts, const char* filename)
        : opts(opts), dfa(std::move(dfa)), filename(filename){};
    void Generate();

    // void GenerateSimd();

  private:
    CodegenOpts opts;
    std::unique_ptr<FiniteAutomaton> dfa;
    const char* filename;

    void get_string_chain(std::shared_ptr<FiniteAutomatonState> to, uint32_t i,
                          std::stringstream& ss);
    void GenerateUtf32(std::ofstream& fs);
    void GenerateDebuggingMain(std::ofstream& fs);
    void GenerateTraverse(std::unique_ptr<FiniteAutomaton> dfa, std::ofstream& fs);
    void GenerateTraverseWhileLoop(std::unique_ptr<FiniteAutomaton> dfa, std::ofstream& fs);
    void GenerateStateSwitchStmt(const FiniteAutomatonState& state, std::ofstream& fs);
    void GenerateState(const FiniteAutomatonState& state, std::ofstream& fs);
  };

}  // namespace ZRegex

#endif  // ZREGEXSTANDALONE_CPP_H
