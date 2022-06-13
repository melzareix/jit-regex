//
// Created by Mohamed El Zarei on 29/12/2021.
//
#include "codegen/cpp.h"

#include <sstream>
#include <vector>

// #include "codegen/cpp-simd.h"
#include "fa/fa.h"

// #define DEBUG 1
namespace ZRegex {
  // void CppCodeGen::GenerateSimd(const std::string &&simd_filename) {
  //   std::ofstream fs;
  //   // file is temporary compiled
  //   auto fp = fmt::format("/tmp/{}.cpp", simd_filename);
  //   fs.open(fp);
  //   if (opts.IsUTF32()) {
  //     CppCodeGen::GenerateUtf32(fs);
  //   }
  // }

  void CppCodeGen::Generate() {
    std::ofstream fs;
    // file is temporary compiled
    auto fp = fmt::format("/tmp/{}.cpp", filename);
    fs.open(fp);
    // std::string s = "special packages";
    // GenerateSIMDCode(fs, s);
    if (opts.IsUTF32()) {
      CppCodeGen::GenerateUtf32(fs);
    }
    // CppCodeGen::GenerateTraverseWhileLoop(std::move(dfa), fs);
    CppCodeGen::GenerateTraverse(std::move(dfa), fs);
#ifdef DEBUG
    CppCodeGen::GenerateDebuggingMain(fs);
#endif
  }

  void CppCodeGen::GenerateDebuggingMain(std::ofstream &fs) {
    fs << "#include <iostream>" << std::endl;
    fs << "#include <x86intrin.h>" << std::endl;
    fs << "#include <string>" << std::endl;
    fs << "int main(int argc, char const *argv[]) {";
    fs << "std::string s;";
    fs << "std::cout << \"Enter text to match: \";";
    fs << "getline(std::cin, s);";
    fs << "bool r = traverse(s.c_str(), s.size());";
    fs << "std::cout <<"
       << "r"
       << "<< std::endl;";
    fs << "return 0;";
    fs << "}" << std::endl;
  }
  void CppCodeGen::GenerateUtf32(std::ofstream &fs) {
    fs << "constexpr unsigned clz(unsigned char a) {";
    fs << "  return __builtin_clz(a) - (8 * (sizeof(unsigned) - 1));";
    fs << "}" << std::endl;

    fs << "inline unsigned readMultiByte(const char* reader, const char& firstByte, unsigned& idx, "
          "unsigned int& byteLen) {";
    fs << "switch (byteLen) {";
    fs << "  case 2:";
    fs << "    return ((firstByte & 0x1F) << 6) | (reader[idx + 1] & 0x3F);";
    fs << "  case 3:";
    fs << "    return ((firstByte & 0xF) << 12) | ((reader[idx + 1] & 0x3F) << 6) | (reader[idx + "
          "2] & 0x3F);";
    fs << "  case 4:";
    fs << "    return ((firstByte & 0x7) << 18) | ((reader[idx + 1] & 0x3F) << 12) | ((reader[idx "
          "+ 2] & 0x3F) << 6)";
    fs << "           | (reader[idx + 3] & 0x3F);";
    fs << "  default:";
    fs << "    return firstByte;";
    fs << " }";
    fs << "}" << std::endl;

    fs << "inline unsigned multiByteSequenceLength(const char& firstByte)";
    fs << "{";
    fs << "  unsigned len = clz(~firstByte);";
    fs << "  return len ? len : 1;";
    fs << "}" << std::endl;

    fs << "inline unsigned nextByte(const char *str, unsigned &idx) {";
    fs << "  unsigned c = 0;";
    fs << "  char b1 = str[idx];";
    fs << "  unsigned byteLen = multiByteSequenceLength(b1);";
    fs << "  c = readMultiByte(str, b1, idx, byteLen);";
    fs << "  idx += byteLen;";
    fs << "  return c;";
    fs << "}" << std::endl;
  }
  void CppCodeGen::GenerateTraverseWhileLoop(std::unique_ptr<FiniteAutomaton> dfa,
                                             std::ofstream &fs) {
    fs << "xpbool traverse(const char* str, unsigned int n) {\n";
    fs << "unsigned int idx = 0;" << std::endl;
    fs << "unsigned int state = " << dfa->initial_state->id << ";" << std::endl;

    auto states = dfa->GetStates();

    fs << "while (idx < n) {" << std::endl;
    if (opts.IsUTF32()) {
      fs << "  unsigned int c = nextByte(str, idx);" << std::endl;
    } else {
      fs << "  unsigned char c = str[idx++];" << std::endl;
    }
    fs << "  switch (state) {" << std::endl;
    for (const auto &state : states) {
      // accept state not needed just return true if transition is to one
      // since we are only interested in matching
      if (state->accept) continue;
      CppCodeGen::GenerateStateSwitchStmt(*state, fs);
    }
    fs << "    default: return false;" << std::endl;
    fs << "  }" << std::endl;
    fs << "}" << std::endl;
    fs << "  return false;" << std::endl;
    fs << "}" << std::endl;
  }
  void CppCodeGen::GenerateStateSwitchStmt(const FiniteAutomatonState &state, std::ofstream &fs) {
    bool first = true;
    fs << "    case " << state.id << ":" << std::endl;
    for (auto &t : state.transitions) {
      auto goto_accept = t.to->accept;
      if (t.min == t.max) {
        if (!first) {
          fs << "      else ";
        }
        fs << "if (c == " << t.min << ") ";
        if (goto_accept) {
          fs << "  return true;" << std::endl;
        } else {
          fs << "  state = " << t.to->id << ";" << std::endl;
        }
      } else {
        if (!first) {
          fs << "      else ";
        }
        fs << "if (c >= " << t.min << " && c <= " << t.max << ") ";
        if (goto_accept) {
          fs << "  return true;" << std::endl;
        } else {
          fs << "  state = " << t.to->id << ";" << std::endl;
        }
      }
      first = false;
    }
    fs << "      else return false;" << std::endl;
    fs << "      break;" << std::endl;
  }

  // Compares two intervals according to starting times.
  bool compareInterval(std::shared_ptr<FiniteAutomatonState> i1,
                       std::shared_ptr<FiniteAutomatonState> i2) {
    return (i1->id < i2->id);
  }

  void CppCodeGen::GenerateTraverse(std::unique_ptr<FiniteAutomaton> dfa, std::ofstream &fs) {
    fs << "bool traverse(const char* str, unsigned int n) {";
    fs << "unsigned int idx = 0;" << std::endl;

    fs << std::endl;
    if (opts.IsUTF32()) {
      fs << "  unsigned int c;" << std::endl;
    } else {
      fs << "  unsigned char c;" << std::endl;
    }
    // auto states = dfa->GetStates();

    auto states_unordered = dfa->GetStates();
    auto states = std::vector<std::shared_ptr<FiniteAutomatonState>>();
    states.insert(states.end(), states_unordered.begin(), states_unordered.end());

    std::sort(states.begin(), states.end(), compareInterval);

    // Initial State
    CppCodeGen::GenerateState(*dfa->initial_state, fs);

    for (const auto &state : states) {
      if (state->id == dfa->initial_state->id) continue;
      // accept state not needed just return true if transition is to one
      // since we are only interested in matching
      if (state->accept) continue;
      CppCodeGen::GenerateState(*state, fs);
    }
    fs << "}" << std::endl;
  }

  void CppCodeGen::get_string_chain(std::shared_ptr<FiniteAutomatonState> to, uint32_t i,
                                    std::stringstream &ss) {
    if (to->chr == '\0') {
      ss << '\0';
      return;
    };
    if (to->transitions.size() != 1) {
      ss << to->chr;
      return;
    }

    ss << to->chr;
    this->get_string_chain((to->transitions.begin())->to, i + 1, ss);
  }

  void CppCodeGen::GenerateState(const FiniteAutomatonState &state, std::ofstream &fs) {
    fs << "s" << state.id << ": " << std::endl;
    fs << "  if (idx >= n) return false;" << std::endl;
    if (opts.IsUTF32()) {
      fs << " c = nextByte(str, idx);" << std::endl;
    } else {
      fs << "  c = str[idx++];" << std::endl;
    }
    auto ptr = std::make_shared<FiniteAutomatonState>(state);
    std::stringstream ss;
    this->get_string_chain(ptr, 0, ss);
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