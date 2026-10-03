#ifndef RCC_LEXER_CORE_H
#define RCC_LEXER_CORE_H

#include "../utils/string.h"
#include "token.h"
#include <stdlib.h>

typedef struct {
  const_string code;
  size_t code_len;
  size_t pos;
} LexerState;

[[nodiscard]]
bool lexer_is_end(LexerState *state);

[[nodiscard]]
char lexer_peek(LexerState *state);

char lexer_consume(LexerState *state);

[[nodiscard]]
Token payloadless_token(TokenKind kind);

typedef bool Cond(LexerState *);
string lexer_consume_while(LexerState *state, Cond cond);

#endif
