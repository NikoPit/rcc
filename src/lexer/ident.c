#include "ident.h"
#include "../utils/string.h"
#include <ctype.h>

/* Returns weather `c` could be a continuation of an ident. Aka characters after
 * the start. */
[[nodiscard]]
static bool is_ident_continue(char c) {
  return isalnum((unsigned char)c) || c == '_';
}

/* Checks weather `str` is a keyword, and returns its `TokenKind` it is. If not,
 * returns `TokenReserved`. */
[[nodiscard]]
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

[[nodiscard]]
static bool licw_cond(LexerState *state) {
  return is_ident_continue(lexer_peek(state));
}

[[nodiscard]]
Token lex_ident(LexerState *state) {
  auto ident = lexer_consume_while(state, licw_cond);

  auto kind = check_keyword(ident);
  if (kind == TokenReserved /* Is not a keyword */) {
    Token token = {.kind = TokenIdent, .ident = ident};
    return token;
  } else {
    free(ident);
    return payloadless_token(kind);
  }
}

[[nodiscard]]
bool is_ident_start(char c) {
  return isalpha((unsigned char)c) || c == '_';
}
