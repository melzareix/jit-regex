#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class ComplementNode : public IBaseNode {
private:
  IBaseNode* exp;

public:
  ComplementNode(IBaseNode* exp) : exp(exp) {}
  void accept(class IVisitor* v) override { v->visit(this); }
};