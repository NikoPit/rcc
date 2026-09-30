#ifndef RCC_LEXER_INTERNAL_H
#define RCC_LEXER_INTERNAL_H

#include "../utils/array.h"
#include "../utils/bool.h"
#include "../utils/misc.h"
#include "token.h"
#include <string.h>

typedef struct {
  const char *code;
  size_t code_len;
  size_t pos;
} LexerState;

static bool is_end(LexerState *state) { return state->pos >= state->code_len; }

static char peek(LexerState *state) {
  if (is_end(state))
    panic("lexer: peek: out of bounds");

  return state->code[state->pos];
}

static char consume(LexerState *state) {
  if (is_end(state))
    panic("lexer: consume: out of bounds");

  return state->code[state->pos++];
}

static Token payloadless_token(TokenKind kind) {
  Token token = {.kind = kind};

  return token;
}

#endif
