#include "string.h"
#include <string.h>

bool string_equals(const char *lhs, const char *rhs) {
  int result = strcmp(lhs, rhs);

  if (result == 0) {
    return true;
  } else {
    return false;
  }
}
