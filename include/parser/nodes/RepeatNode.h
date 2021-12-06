#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class RepeatNode : public IBaseNode {
private:
  uint32_t min;
  uint32_t max;
  IBaseNode* exp;

public:
  RepeatNode(IBaseNode* exp, uint32_t min, uint32_t max) : exp(exp), min(min), max(max) {}
  void accept(class IVisitor* v) override { v->visit(this); }
};