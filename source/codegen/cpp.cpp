//
// Created by Mohamed El Zarei on 29/12/2021.
//
#include "codegen/cpp.h"

#include <vector>

#include "fa/fa.h"
namespace ZRegex {
  void CppCodeGen::Generate(std::unique_ptr<FiniteAutomaton> dfa, const char *filename) {
    std::ofstream fs;
    // file is temporary compiled
    auto fp = fmt::format("/tmp/{}.cpp", filename);
    fs.open(fp);

    CppCodeGen::GenerateTraverse(std::move(dfa), fs);
  }

  void CppCodeGen::GenerateTraverse(std::unique_ptr<FiniteAutomaton> dfa, std::ofstream &fs) {
    fs << "bool traverse(char* str, unsigned int n) {";
    fs << "unsigned int idx = 0;" << std::endl;
    //    if (dfa->is_singleton()) {
    //      fs << "return str == \"" << dfa->get_accepted_str() << "\"; }" << std::endl;
    //      return;
    //    }

    fs << std::endl;
    fs << "  unsigned int c;" << std::endl;
    auto states = dfa->GetStates();

    for (const auto &state : states) {
      if (state->id == dfa->initial_state->id) continue;
      // accept state not needed just return true if transition is to one
      // since we are only interested in matching
      if (state->accept) continue;
      CppCodeGen::GenerateState(*state, fs);
    }
    fs << "}" << std::endl;
  }

  void CppCodeGen::GenerateState(const FiniteAutomatonState &state, std::ofstream &fs) {
    fs << "s" << state.id << ": " << std::endl;
    fs << "  if (idx >= n) return false;" << std::endl;
    fs << "  c = (unsigned int)(unsigned char)str[idx++];" << std::endl;
    for (auto &t : state.transitions) {
      auto goto_accept = t.to->accept;

      if (t.min == t.max) {
        fs << "if (c ==" << t.min << ") ";
        if (goto_accept) {
          fs << "return true;" << std::endl;
        } else {
          fs << "goto s" << t.to->id << ";" << std::endl;
        }
      } else {
        fs << "if (c >=" << t.min << " && c <= " << t.max << ") ";
        if (goto_accept) {
          fs << "return true;" << std::endl;
        } else {
          fs << "goto s" << t.to->id << ";" << std::endl;
        }
      }
    }

    fs << "return false;" << std::endl;
  }
}  // namespace ZRegex