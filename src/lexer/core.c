#include "core.h"
#include "../utils/misc.h"
#include "../utils/string.h"

bool lexer_is_end(LexerState *state) { return state->pos >= state->code_len; }

char lexer_peek(LexerState *state) {
  if (lexer_is_end(state))
    panic("lexer: peek: out of bounds");

  return state->code[state->pos];
}

char lexer_consume(LexerState *state) {
  if (lexer_is_end(state))
    panic("lexer: consume: out of bounds");

  return state->code[state->pos++];
}

Token payloadless_token(TokenKind kind) {
  Token token = {.kind = kind};

  return token;
}

#define CONSUME_WHILE_BUF_SIZE 256
char *lexer_consume_while(LexerState *state, Cond cond) {
  char text[CONSUME_WHILE_BUF_SIZE];
  size_t size = 0;

  while (!lexer_is_end(state) && cond(state)) {
    text[size] = lexer_consume(state);
    size++;

    if (size >= CONSUME_WHILE_BUF_SIZE)
      panic("lexer: consume_while: size exceeded max size");
  }

  text[size] = '\0';

  return clone_string(text);
}
