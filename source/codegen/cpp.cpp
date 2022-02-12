//
// Created by Mohamed El Zarei on 29/12/2021.
//
#include "codegen/cpp.h"

#include <vector>

#include "fa/fa.h"

//#define DEBUG 1
namespace ZRegex {
  void CppCodeGen::Generate(std::unique_ptr<FiniteAutomaton> dfa, const char *filename,
                            const Encoding &encoding) {
    std::ofstream fs;
    // file is temporary compiled
    auto fp = fmt::format("/tmp/{}.cpp", filename);
    fs.open(fp);
    if (encoding == UTF8) {
      CppCodeGen::GenerateUtf8(fs);
    }
    CppCodeGen::GenerateTraverse(std::move(dfa), fs, encoding);
#ifdef DEBUG
    CppCodeGen::GenerateDebuggingMain(fs);
#endif
  }

  void CppCodeGen::GenerateDebuggingMain(std::ofstream &fs) {
    fs << "#include <iostream>" << std::endl;
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
  void CppCodeGen::GenerateUtf8(std::ofstream &fs) {
    fs << "constexpr unsigned clz(unsigned char a) {";
    fs << "  return __builtin_clz(a) - (8 * (sizeof(unsigned) - 1));";
    fs << "}" << std::endl;

    fs << "unsigned readMultiByte(const char* reader, const char& firstByte, unsigned& idx, "
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

    fs << "unsigned nextByte(const char *str, unsigned &idx) {";
    fs << "  unsigned c = 0;";
    fs << "  char b1 = str[idx];";
    fs << "  unsigned byteLen = multiByteSequenceLength(b1);";
    fs << "  c = readMultiByte(str, b1, idx, byteLen);";
    fs << "  idx += byteLen;";
    fs << "  return c;";
    fs << "}" << std::endl;
  }
  void CppCodeGen::GenerateTraverse(std::unique_ptr<FiniteAutomaton> dfa, std::ofstream &fs,
                                    const Encoding &encoding) {
    fs << "bool traverse(const char* str, unsigned int n) {";
    fs << "unsigned int idx = 0;" << std::endl;

    fs << std::endl;
    fs << "  unsigned int c;" << std::endl;
    auto states = dfa->GetStates();

    // Initial State
    CppCodeGen::GenerateState(*dfa->initial_state, fs, encoding);

    for (const auto &state : states) {
      if (state->id == dfa->initial_state->id) continue;
      // accept state not needed just return true if transition is to one
      // since we are only interested in matching
      if (state->accept) continue;
      CppCodeGen::GenerateState(*state, fs, encoding);
    }
    fs << "}" << std::endl;
  }

  void CppCodeGen::GenerateState(const FiniteAutomatonState &state, std::ofstream &fs,
                                 const Encoding &encoding) {
    fs << "s" << state.id << ": " << std::endl;
    fs << "  if (idx >= n) return false;" << std::endl;
    if (encoding == UTF8) {
      fs << " c = nextByte(str, idx);" << std::endl;
    } else {
      fs << "  c = (unsigned int)(unsigned char)str[idx++];" << std::endl;
    }
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