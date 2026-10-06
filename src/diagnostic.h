#pragma once

#include "misc.h"
#include "utils/string.h"

[[noreturn]]
void diag_error(const CompileContext *ctx, Span span, const_string message);

void diag_warning(const CompileContext *ctx, Span span, const_string message);
