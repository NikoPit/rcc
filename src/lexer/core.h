#pragma once

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

/* Discardable */
char lexer_consume(LexerState *state);

[[nodiscard]]
Token payloadless_token(TokenKind kind);

typedef bool Cond(LexerState *);
/* Discardable */
string lexer_consume_while(LexerState *state, Cond cond);
