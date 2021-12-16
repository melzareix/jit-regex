#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class CharRangeNode : public IBaseNode {
private:
  char start_;
  char end_;

public:
  CharRangeNode(char start, char end) : start_(start), end_(end) {}
  void accept(class IVisitor* v) override { v->visit(this); }

  [[nodiscard]] const char& start() const { return start_; }
  [[nodiscard]] const char& end() const { return end_; }
};