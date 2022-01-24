#pragma once

#include <iostream>

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
#include "IVisitor.h"
#include "fa/factory.h"
#include "nodes/IBaseNode.h"

class AutomataVisitor
    : public ValueGetter<AutomataVisitor, IBaseNode, std::unique_ptr<ZRegex::FiniteAutomaton>>,
      public IVisitor {
private:
public:
  void visit(CharRangeNode *v) override {
    Return(ZRegex::FAFactory::CharRangeAutomaton(v->start(), v->end()));
  }
  void visit(ComplementNode *v) override { std::cout << "complement!" << std::endl; }
  void visit(ConcatNode *v) override {
    auto lhs_automaton = GetValue(v->get_lhs());
    auto rhs_automaton = GetValue(v->get_rhs());

    Return(ZRegex::FAFactory::Concat(std::move(lhs_automaton), std::move(rhs_automaton)));
  }
  void visit(IntersectionNode *v) override {}
  void visit(OptionalNode *v) override {
    auto exp_automaton = GetValue(v->exp());
    Return(ZRegex::FAFactory::Optional(std::move(exp_automaton )));
  }
  void visit(AnyCharNode *v) override {
    Return(ZRegex::FAFactory::CharRangeAutomaton(0, 127));
  }
  void visit(AutomataNode *v) override {}
  void visit(CharNode *v) override { std::cout << "ok"; }
  void visit(RepeatNode *v) override {
    auto exp_automaton = GetValue(v->exp());
    Return(ZRegex::FAFactory::RepeatMinimum(std::move(exp_automaton), v->min()));
  }
  void visit(StringNode *v) override { Return(ZRegex::FAFactory::StringAutomaton(v->get_str())); }
  void visit(UnionNode *v) override {
    auto lhs_automaton = GetValue(v->get_lhs());
    auto rhs_automaton = GetValue(v->get_rhs());

    Return(ZRegex::FAFactory::Union(std::move(lhs_automaton), std::move(rhs_automaton)));
  }
};
