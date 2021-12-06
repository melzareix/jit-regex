#pragma once

#include <iostream>

class IBaseNode {
public:
  virtual void accept(class IVisitor *v) = 0;
};
