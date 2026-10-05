#pragma once

#include "../misc.h"
#include "../utils/array.h"
#include "token.h"

[[nodiscard]]
array_t(Token) lex(CompileContext *ctx);
void free_tokens(array_t(Token) tokens);
