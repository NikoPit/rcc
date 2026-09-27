#include "misc.h"

#include <stdio.h>
#include <stdlib.h>

RCC_NORETURN void fail(const char *message) {
  fputs(message, stderr);
  abort();
}

RCC_NORETURN void unreachable() { fail("Unreachable statement reached"); }
