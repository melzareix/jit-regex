#pragma once

#include "../automaton/AutomatonFactory.h"
#include "../parser/IVisitor.h"
#include "../parser/nodes/AnyCharNode.h"
#include "../parser/nodes/CharNode.h"
#include "../parser/nodes/CharRangeNode.h"
#include "../parser/nodes/ComplementNode.h"
#include "../parser/nodes/ConcatNode.h"
#include "../parser/nodes/IntersectionNode.h"
#include "../parser/nodes/OptionalNode.h"
#include "../parser/nodes/RepeatNode.h"
#include "../parser/nodes/StringNode.h"
#include "../parser/nodes/UnionNode.h"

#include <iostream>

#include "../automaton/AutomatonFactory.h"
#include "IVisitor.h"
#include "nodes/IBaseNode.h"

class AutomataVisitor : public ValueGetter<AutomataVisitor, IBaseNode, Automaton *>,
                        public IVisitor {
private:

public:
  void visit(CharRangeNode *v)  override{
    Return(AutomatonFactory::create_char_range_automaton(v->start(), v->end()));
  }
  void visit(ComplementNode *v) override{ std::cout << "complement!" << std::endl; }
  void visit(ConcatNode *v) override {
    auto lhs_automaton = GetValue(v->get_lhs());
    auto rhs_automaton = GetValue(v->get_rhs());

    Return(AutomatonFactory::create_concat_automaton(lhs_automaton, rhs_automaton));
  }
  void visit(IntersectionNode *v) override {}
  void visit(OptionalNode *v) override { std::cout << "optional" << std::endl; }
  void visit(AnyCharNode *v) override {
    //TODO(melzarei) handling unicode related issues
    Return(AutomatonFactory::create_char_range_automaton('\u0000', (char)(128)));
  }
  void visit(AutomataNode *v) override {}
  void visit(CharNode *v) override { std::cout << "ok"; }
  void visit(RepeatNode *v) override {
    auto exp_automaton = GetValue(v->exp());
    Return(AutomatonFactory::create_kleene_star_automata(exp_automaton));
  }
  void visit(StringNode *v) override {
    Return(AutomatonFactory::create_string_automaton(v->get_str()));
  }
  void visit(UnionNode *v) override { std::cout << "uuuuu" << std::endl; }
};
