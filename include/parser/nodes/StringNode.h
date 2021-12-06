#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class StringNode : public IBaseNode {
private:
  const std::string str;

public:
  StringNode(std::string str) : str(str) {}
  void accept(IVisitor* v) override { v->visit(this); }
  std::string get_str() { return str; }
};