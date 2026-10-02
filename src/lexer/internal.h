#ifndef RCC_LEXER_INTERNAL_H
#define RCC_LEXER_INTERNAL_H

#include "../utils/array.h"
#include "../utils/bool.h"
#include "../utils/misc.h"
#include "../utils/string.h"
#include "token.h"

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

typedef bool Cond(LexerState *);
#define CONSUME_WHILE_BUF_SIZE 256
static char *consume_while(LexerState *state, Cond cond) {
  char text[CONSUME_WHILE_BUF_SIZE];
  size_t size = 0;

  while (!is_end(state) && cond(state)) {
    text[size] = consume(state);
    size++;

    if (size >= CONSUME_WHILE_BUF_SIZE)
      panic("lexer: consume_while: size exceeded max size");
  }

  text[size] = '\0';

  return clone_string(text);
}

#endif
