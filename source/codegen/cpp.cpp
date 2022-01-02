//
// Created by Mohamed El Zarei on 29/12/2021.
//
#include "codegen/cpp.h"

#include <vector>
namespace ZRegex {
  void CppCodeGen::Generate(Automaton *dfa, const char *filename) {
    std::ofstream fs;
    // file is temporary compiled
    auto fp = fmt::format("/tmp/{}.cpp", filename);
    fs.open(fp);

    CppCodeGen::GenerateTraverse(dfa, fs);
  }

  void CppCodeGen::GenerateTraverse(Automaton *dfa, std::ofstream &fs) {
    fs << "bool traverse(char* str, unsigned int n) {";
    fs << "unsigned int idx = 0;" << std::endl;
    if (dfa->is_singleton()) {
      fs << "return str == \"" << dfa->get_accepted_str() << "\"; }" << std::endl;
      return;
    }

    fs << std::endl;
    fs << "  unsigned int c;" << std::endl;
    auto accept_states = dfa->accept_states();
    CppCodeGen::GenerateState(dfa->states[dfa->initial()], fs, accept_states);

    for (auto state : dfa->reachable_states()) {
      if (state == dfa->initial()) continue;
      // accept state not needed just return true if transition is to one
      // since we are only interested in matching
      if (dfa->states[state].is_accept()) continue;
      CppCodeGen::GenerateState(dfa->states[state], fs, accept_states);
    }
    fs << "}" << std::endl;
  }

  void CppCodeGen::GenerateState(const State &state, std::ofstream &fs,
                                 const std::vector<uint32_t> &accept_states) {
    fs << "s" << state.id() << ": " << std::endl;
    fs << "  if (idx >= n) return false;" << std::endl;
    fs << "  c = (unsigned int)(unsigned char)str[idx++];" << std::endl;
    for (auto &t : state.transitions()) {
      auto goto_accept
          = std::find(accept_states.begin(), accept_states.end(), t.dest()) != accept_states.end();

      if (t.min() == t.max()) {
        fs << "if (c ==" << t.min() << ") ";
        if (goto_accept) {
          fs << "return true;" << std::endl;
        } else {
          fs << "goto s" << t.dest() << ";" << std::endl;
        }
      } else {
        fs << "if (c >=" << t.min() << " && c <= " << t.max() << ") ";
        if (goto_accept) {
          fs << "return true;" << std::endl;
        } else {
          fs << "goto s" << t.dest() << ";" << std::endl;
        }
      }
    }

    fs << "return false;" << std::endl;
  }
}  // namespace ZRegex