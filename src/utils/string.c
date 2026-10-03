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

[[nodiscard]]
string clone_string(const_string str) {
  auto len = strlen(str) + 1; /* +1 for the null termiator */

  auto cloned = malloc(len);
  if (cloned == nullptr)
    panic("clone_string: out of memory");

  memcpy(cloned, str, len);

  return cloned;
}
