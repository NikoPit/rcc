#ifndef RCC_LEXER_CORE_H
#define RCC_LEXER_CORE_H

#include "../utils/bool.h"
#include "token.h"
#include <stdlib.h>

typedef struct {
  const char *code;
  size_t code_len;
  size_t pos;
} LexerState;

bool is_end(LexerState *state);

char peek(LexerState *state);
char consume(LexerState *state);

Token payloadless_token(TokenKind kind);

typedef bool Cond(LexerState *);
char *consume_while(LexerState *state, Cond cond);

#endif
