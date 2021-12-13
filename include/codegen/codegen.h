//
// Created by Mohamed ElZarei on 13/12/2021.
//

#pragma once
#include <fstream>
#include <string>

#include "../automaton/Automaton.h"
#include "../parser/AutomataVisitor.h"
#include "../parser/RegExp.h"

class Codegen {
public:
  static void gen_regex(const std::string& pattern) {
    RegExp regExp(pattern);
    auto node = regExp.parse();

    // evaluate ast and create the automata
    auto res = AutomataVisitor::GetValue(node);
    if (!res->is_dfa()) {
      throw std::runtime_error("Failure! Cannot generate code from NFA must be DFA!.");
    }
    std::ofstream fs;
    fs.open("./build/codegen_test.cpp");

    headers_(fs);
    traverse_(res, fs);
    main_(fs);

    fs.close();
  }
  static void main_(std::ofstream& fs) {
    fs << "int main(int argc, char* argv[]) {" << std::endl;
    fs << "  // load data" << std::endl;
    fs << "  std::string str(argv[1]);" << std::endl;

    fs << "  std::ifstream st(str, std::ios_base::binary);" << std::endl;
    fs << std::endl;

    fs << "  int total_dfa_time = 0;" << std::endl;
    fs << "  int dfa_lines = 0;" << std::endl;
    fs << "  int matched = 0;" << std::endl;
    fs << "  std::string line;" << std::endl;

    fs << "  auto total_time_start = high_resolution_clock::now();" << std::endl;
    fs << "  while(std::getline(st, line)) {" << std::endl;
    fs << "    dfa_lines++;" << std::endl;
    fs << "    long unsigned int idx = 0;" << std::endl;
    fs << "    auto start = high_resolution_clock::now();" << std::endl;
    fs << "    int m = traverse(idx, line);" << std::endl;
    fs << "    auto end = high_resolution_clock::now();" << std::endl;
    fs << "    matched += m;" << std::endl;
    fs << "    total_dfa_time += (duration_cast<microseconds>(end - start)).count();" << std::endl;
    fs << "  }" << std::endl;

    fs << "  auto total_time_end = high_resolution_clock::now();" << std::endl;
    fs << "  std::cout << \"Dfa_num_of_matched_lines: \" << matched << \" out of \" << dfa_lines "
          "<< std::endl;"
       << std::endl;

    fs << "  std::cout << \"DFA_time_to_exec: \" << total_dfa_time << \" microseconds. Lines: \" "
          "<< "
          "dfa_lines << std::endl;"
       << std::endl;
    fs << "  std::cout << \"Total Loop Exec time: \" << "
          "(duration_cast<microseconds>(total_time_end "
          "- total_time_start).count()) / (1e6) << \" seconds.\" << std::endl;"
       << std::endl;
    fs << std::endl;

    // end main
    fs << "}" << std::endl;
    fs << std::endl;
  }
  static void state_(std::ofstream& fs, State& s) {
    // accept states
    if (s.is_accept()) {
      fs << "// accept state" << std::endl;
      fs << "s" << s.id() << ":"
         << "  return true;" << std::endl;
      return;
    }

    // regular states
    fs << "s" << s.id() << ": " << std::endl;
    fs << "  if (idx >= str.size()) return false;" << std::endl;
    fs << "  c = (unsigned int)(unsigned char)str[idx++];" << std::endl;
    for (auto t : s.transitions()) {
      if (t.min() == t.max()) {
        fs << "if (c ==" << t.min() << ") "
           << "goto s" << t.dest() << ";" << std::endl;
      } else {
        fs << "if (c >=" << t.min() << " && c <= " << t.max() << ") "
           << "goto s" << t.dest() << ";" << std::endl;
      }
    }

    fs << "return false;" << std::endl;
  }

  static void headers_(std::ofstream& fs) {
    fs << "// #######################################################################" << std::endl;
    fs << "// AUTOGENERATED FILE DON'T MODIFY." << std::endl;
    fs << "// #######################################################################" << std::endl;
    fs << "#include <iostream>" << std::endl;
    fs << "#include <string>" << std::endl;
    fs << "#include <fstream>" << std::endl;
    fs << "#include <sstream>" << std::endl;
    fs << "#include <chrono>" << std::endl;
    fs << "using namespace std::chrono;" << std::endl;
    fs << std::endl;
  }
  static void traverse_(Automaton* aut, std::ofstream& fs) {
    fs << "bool traverse(long unsigned int idx, std::string &str) {";

    if (aut->is_singleton()) {
      fs << "return str == \"" << aut->get_accepted_str() << "\"; }" << std::endl;
      return;
    }

    fs << std::endl;
    fs << "  unsigned int c;" << std::endl;
    state_(fs, aut->states[aut->initial()]);
    for (auto state : aut->states) {
      if (state.first == aut->initial()) continue;
      state_(fs, state.second);
    }

    fs << "}" << std::endl;
  }
};