#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

#include <string>

class AutomataNode : public IBaseNode {
private:
  std::string str;

public:
  AutomataNode(std::string str) : str(str) {}
  void accept(class IVisitor* v) override { v->visit(this); }
};