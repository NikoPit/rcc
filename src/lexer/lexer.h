#pragma once

#include "../utils/array.h"
#include "token.h"

[[nodiscard]]
array_t(Token) lex(const_string code);
void free_tokens(array_t(Token) tokens);
