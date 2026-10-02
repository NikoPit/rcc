#include "ident.h"
#include "../utils/string.h"
#include <ctype.h>

/* Returns weather `c` could be a continuation of an ident. Aka characters after
 * the start. */
static bool is_ident_continue(char c) {
  return isalnum((unsigned char)c) || c == '_';
}

/* Checks weather `str` is a keyword, and returns its `TokenKind` it is. If not,
 * returns `TokenReserved`. */
static TokenKind check_keyword(const char *str) {
  if (string_equals(str, "int")) {
    return TokenInt;
  } else if (string_equals(str, "void")) {
    return TokenVoid;
  } else if (string_equals(str, "return")) {
    return TokenReturn;
  } else {
    return TokenReserved;
  }
}

static bool licw_cond(LexerState *state) {
  return is_ident_continue(lexer_peek(state));
}

Token lex_ident(LexerState *state) {
  char *ident = lexer_consume_while(state, licw_cond);

  TokenKind kind = check_keyword(ident);
  if (kind == TokenReserved /* Is not a keyword */) {
    Token token = {.kind = TokenIdent, .ident = ident};
    return token;
  } else {
    free(ident);
    return payloadless_token(kind);
  }
}

bool is_ident_start(char c) { return isalpha((unsigned char)c) || c == '_'; }
