#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class AnyStringNode : public IBaseNode {
public:
  AnyStringNode() {}
  void accept(class IVisitor* v) override { v->visit(this); }
};