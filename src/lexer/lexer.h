#ifndef RCC_LEXER_H
#define RCC_LEXER_H

#include "token.h"

[[nodiscard]]
Token *lex(const char *code);
void free_tokens(Token *tokens);

#endif
