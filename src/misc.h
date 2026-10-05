#pragma once

#include "utils/string.h"
#include <stddef.h>

typedef struct {
  const_string source;
  size_t source_len;
  const_string file_name;
} CompileContext;
