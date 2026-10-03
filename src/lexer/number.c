#include "number.h"
#include "core.h"
#include <ctype.h>
#include <stdlib.h>

static bool lncw_cond(LexerState *state) {
  return isdigit((unsigned char)lexer_peek(state));
}

Token lex_number(LexerState *state) {
  auto number_str = lexer_consume_while(state, lncw_cond);

  Token token = {.kind = TokenNumber, .number = atoll(number_str)};
  free(number_str);
  return token;
}
