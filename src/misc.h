#pragma once

#include "utils/string.h"
#include <stddef.h>

typedef struct {
  size_t start;
  size_t len;
} Span;

typedef struct {
  const_string source;
  size_t source_len;
  const_string file_name;
} CompileContext;
