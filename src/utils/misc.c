#include "misc.h"

#include <stdio.h>
#include <stdlib.h>

[[noreturn]]
void panic(const_string message) {
  fputs(message, stderr);
  fputc('\n', stderr);

  abort();
}
