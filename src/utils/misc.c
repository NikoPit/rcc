#include "misc.h"

#include <stdio.h>
#include <stdlib.h>

void fail(const char *message) {
  fputs(message, stderr);
  abort();
}

void unreachable() { fail("Unreachable statement reached"); }
