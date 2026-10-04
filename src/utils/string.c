#include "string.h"
#include "misc.h"
#include <stdlib.h>
#include <string.h>

[[nodiscard]]
bool string_equals(const_string lhs, const_string rhs) {
  auto result = strcmp(lhs, rhs);

  if (result == 0) {
    return true;
  } else {
    return false;
  }
}
