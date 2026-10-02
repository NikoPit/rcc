#ifndef RCC_LEXER_CORE_H
#define RCC_LEXER_CORE_H

#include "token.h"
#include <stdlib.h>

typedef struct {
  const char *code;
  size_t code_len;
  size_t pos;
} LexerState;

bool lexer_is_end(LexerState *state);

char lexer_peek(LexerState *state);
char lexer_consume(LexerState *state);

Token payloadless_token(TokenKind kind);

typedef bool Cond(LexerState *);
char *lexer_consume_while(LexerState *state, Cond cond);

#endif
