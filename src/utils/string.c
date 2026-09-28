#include "string.h"
#include "misc.h"
#include <stdlib.h>
#include <string.h>

bool string_equals(const char *lhs, const char *rhs) {
  int result = strcmp(lhs, rhs);

  if (result == 0) {
    return true;
  } else {
    return false;
  }
}

char *clone_string(const char *string) {
  size_t len = strlen(string) + 1; /* +1 for the null termiator */

  char *cloned = malloc(len);
  if (cloned == NULL)
    panic("clone_string: out of memory");

  memcpy(cloned, string, len);

  return cloned;
}
