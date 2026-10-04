#ifndef RCC_MISC_H
#define RCC_MISC_H

#include "../utils/string.h"

[[noreturn]]
void panic(const_string message);

#define new_zeroed(type)                                                       \
  (type) {}

#endif
