#pragma once

#include "../../utility-classes/object.h"

/* author:jvitek */
class Num : public Object {
public:
  size_t v = 0;
  Num() {}
  Num(size_t v) : v(v) {}
};
