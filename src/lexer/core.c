#include "core.h"
#include "../utils/misc.h"
#include "../utils/string.h"

[[nodiscard]]
bool lexer_is_end(LexerState *state) {
  return state->pos >= state->ctx->source_len;
}

[[nodiscard]]
char lexer_peek(LexerState *state) {
  if (lexer_is_end(state))
    panic("lexer: peek: out of bounds");

  return state->ctx->source[state->pos];
}

/* Discardable */
char lexer_consume(LexerState *state) {
  if (lexer_is_end(state))
    panic("lexer: consume: out of bounds");

  return state->ctx->source[state->pos++];
}

[[nodiscard]]
Token payloadless_token(TokenKind kind) {
  return (Token){.kind = kind};
}

/* Discardable */
string lexer_consume_while(LexerState *state, Cond cond) {
  constexpr auto buf_size = 256;
  auto text = new_zeroed(char[buf_size]);
  auto size = 0;

  while (!lexer_is_end(state) && cond(state)) {
    text[size] = lexer_consume(state);
    size++;

    if (size >= buf_size)
      panic("lexer: consume_while: size exceeded max size");
  }

  text[size] = '\0';

  return strdup(text);
}
