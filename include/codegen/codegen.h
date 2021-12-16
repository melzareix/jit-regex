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
  static void main_(std::ofstream& fs);
  static void state_(std::ofstream& fs, State& s);
  static void headers_(std::ofstream& fs);
  static void traverse_(Automaton* aut, std::ofstream& fs);

public:
  static void generate(const std::string& pattern);
};