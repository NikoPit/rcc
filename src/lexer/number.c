#include "number.h"
#include "core.h"
#include <ctype.h>
#include <stdlib.h>

[[nodiscard]]
static bool lncw_cond(LexerState *state) {
  return isdigit((unsigned char)lexer_peek(state));
}

[[nodiscard]]
Token lex_number(LexerState *state) {
  auto number_str = lexer_consume_while(state, lncw_cond);

  auto token = (Token){.kind = TokenNumber, .number = atoll(number_str)};
  free(number_str);
  return token;
}
