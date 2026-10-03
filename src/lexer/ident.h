#ifndef RCC_LEXER_IDENT_H
#define RCC_LEXER_IDENT_H

#include "core.h"
#include "token.h"

[[nodiscard]]
Token lex_ident(LexerState *state);

/* Returns weather `c` could be the start of an ident. */
[[nodiscard]]
bool is_ident_start(char c);

#endif
