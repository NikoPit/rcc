#ifndef RCC_LEXER_H
#define RCC_LEXER_H

#include "../utils/array.h"
#include "token.h"

[[nodiscard]]
array_t(Token) lex(const_string code);
void free_tokens(array_t(Token) tokens);

#endif
