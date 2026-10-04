#pragma once

#include "../utils/string.h"

[[noreturn]]
void panic(const_string message);

#define new_zeroed(type)                                                       \
  (type) {}
