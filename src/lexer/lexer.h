#ifndef RCC_LEXER_H
#define RCC_LEXER_H

#include "../utils/array.h"

Array lex(const char *code);
void free_tokens(Array *tokens);

#endif
