#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class EmptyNode : public IBaseNode {
public:
  EmptyNode() {}
  void accept(class IVisitor* v) override { v->visit(this); }
};