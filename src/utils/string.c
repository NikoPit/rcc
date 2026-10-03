#include "string.h"
#include "misc.h"
#include <stdlib.h>
#include <string.h>

bool string_equals(const char *lhs, const char *rhs) {
  auto result = strcmp(lhs, rhs);

  if (result == 0) {
    return true;
  } else {
    return false;
  }
}

char *clone_string(const char *string) {
  auto len = strlen(string) + 1; /* +1 for the null termiator */

  auto cloned = malloc(len);
  if (cloned == NULL)
    panic("clone_string: out of memory");

  memcpy(cloned, string, len);

  return cloned;
}
