#include "misc.h"

#include <stdio.h>
#include <stdlib.h>

[[noreturn]]
void panic(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);

  abort();
}
