#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "core.h"
#include "ident.h"
#include "lexer.h"
#include "misc.h"
#include "number.h"
#include "token.h"

#include "../utils/misc.h"

#include "../../libs/stb_ds.h"

[[nodiscard]]
static bool should_skip(char c) {
  return c == '\n' || c == '\t' || c == '\r' || c == ' ';
}

[[nodiscard]]
static Token next(LexerState *state) {
  while (!lexer_is_end(state) && should_skip(lexer_peek(state)))
    lexer_consume(state);

  if (lexer_is_end(state))
    return payloadless_token(TokenEnd);

  auto c = lexer_peek(state);

  if (is_ident_start(c)) {
    return lex_ident(state);
  } else if (isdigit((unsigned char)c)) {
    return lex_number(state);
  } else {
    return lex_misc(state);
  }
}

[[nodiscard]]
array_t(Token) lex(const_string code) {
  auto state = (LexerState){.code = code, .code_len = strlen(code), .pos = 0};
  auto tokens = create_array(Token);

  while (true) {
    auto next_token = next(&state);
    if (next_token.kind == TokenEnd)
      break;

    arrput(tokens, next_token);
  }

  return tokens;
}

void free_tokens(array_t(Token) tokens) {
  /* Free the cloned idents */
  for (auto i = (ptrdiff_t)0; i < arrlen(tokens); i++) {
    auto token = tokens[i];
    if (token.kind == TokenIdent)
      free(token.ident);
  }

  arrfree(tokens);
}
