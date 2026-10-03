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

#include "../../libs/stb_ds.h"

static bool should_skip(char c) {
  return c == '\n' || c == '\t' || c == '\r' || c == ' ';
}

static Token next(LexerState *state) {
  while (!lexer_is_end(state) && should_skip(lexer_peek(state)))
    lexer_consume(state);

  if (lexer_is_end(state))
    return payloadless_token(TokenEnd);

  char c = lexer_peek(state);

  if (is_ident_start(c)) {
    return lex_ident(state);
  } else if (isdigit((unsigned char)c)) {
    return lex_number(state);
  } else {
    return lex_misc(state);
  }
}

Token *lex(const char *code) {
  LexerState state = {.code = code, .code_len = strlen(code), .pos = 0};
  Token *tokens = NULL;

  while (true) {
    Token next_token = next(&state);
    if (next_token.kind == TokenEnd)
      break;

    arrput(tokens, next_token);
  }

  return tokens;
}

void free_tokens(Token *tokens) {
  /* Free the cloned idents */
  for (ptrdiff_t i = 0; i < arrlen(tokens); i++) {
    Token token = tokens[i];
    if (token.kind == TokenIdent)
      free(token.ident);
  }

  arrfree(tokens);
}
