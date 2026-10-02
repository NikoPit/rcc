#include "core.h"
#include "../utils/misc.h"
#include "../utils/string.h"

bool is_end(LexerState *state) { return state->pos >= state->code_len; }

char peek(LexerState *state) {
  if (is_end(state))
    panic("lexer: peek: out of bounds");

  return state->code[state->pos];
}

char consume(LexerState *state) {
  if (is_end(state))
    panic("lexer: consume: out of bounds");

  return state->code[state->pos++];
}

Token payloadless_token(TokenKind kind) {
  Token token = {.kind = kind};

  return token;
}

#define CONSUME_WHILE_BUF_SIZE 256
char *consume_while(LexerState *state, Cond cond) {
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
