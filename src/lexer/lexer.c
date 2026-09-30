#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../utils/bool.h"
#include "../utils/misc.h"
#include "../utils/string.h"
#include "internal.h"
#include "lexer.h"
#include "token.h"

static bool should_skip(char c) {
  return c == '\n' || c == '\t' || c == '\r' || c == ' ';
}

/* Returns weather `c` could be the start of an ident. */
static bool is_ident_start(char c) {
  return isalpha((unsigned char)c) || c == '_';
}

/* Returns weather `c` could be a continuation of an ident. Aka characters after
 * the start. */
static bool is_ident_continue(char c) {
  return isalnum((unsigned char)c) || c == '_';
}

/* Checks weather `str` is a keyword, and returns its `TokenKind` it is. If not,
 * returns `TokenReserved`. */
static TokenKind check_keyword(char *str) {
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

#define IDENT_BUFFER_SIZE 256
static Token lex_ident(LexerState *state) {
  char ident[IDENT_BUFFER_SIZE] = {};
  size_t size = 0;

  while (!is_end(state) && is_ident_continue(peek(state))) {
    ident[size] = consume(state);
    size++;

    if (size >= IDENT_BUFFER_SIZE)
      panic(
          "lexer: lex_ident: ident size exceeded max size (IDENT_BUFFER_SIZE)");
  }

  ident[size] = '\0';

  TokenKind kind = check_keyword(ident);
  if (kind == TokenReserved) {
    Token token = {.kind = TokenIdent, .ident = clone_string(ident)};
    return token;
  } else {
    return payloadless_token(kind);
  }
}

static Token lex_misc(LexerState *state) {
  switch (consume(state)) {
  case '{':
    return payloadless_token(TokenLeftBrace);
  case '}':
    return payloadless_token(TokenRightBrace);
    break;
  case '(':
    return payloadless_token(TokenLeftParen);
  case ')':
    return payloadless_token(TokenRightParen);
  default:
    panic("lexer: unknown token");
  }
}

static Token next(LexerState *state) {
  while (!is_end(state) && should_skip(peek(state)))
    consume(state);

  if (is_end(state))
    return payloadless_token(TokenEnd);

  char c = peek(state);

  if (is_ident_start(c)) {
    return lex_ident(state);
  } else {
    return lex_misc(state);
  }
}

Array lex(const char *code) {
  LexerState state = {.code = code, .code_len = strlen(code), .pos = 0};
  Array tokens = create_array(Token);

  while (true) {
    Token next_token = next(&state);
    if (next_token.kind == TokenEnd)
      break;

    array_push(&tokens, Token, next_token);
  }

  return tokens;
}
