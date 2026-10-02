#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../utils/bool.h"
#include "../utils/misc.h"
#include "core.h"
#include "ident.h"
#include "lexer.h"
#include "token.h"

static bool should_skip(char c) {
  return c == '\n' || c == '\t' || c == '\r' || c == ' ';
}

static Token lex_misc(LexerState *state) {
  switch (lexer_consume(state)) {
  case '{':
    return payloadless_token(TokenLeftBrace);
  case '}':
    return payloadless_token(TokenRightBrace);
    break;
  case '(':
    return payloadless_token(TokenLeftParen);
  case ')':
    return payloadless_token(TokenRightParen);
  case ';':
    return payloadless_token(TokenSemicolon);
  default:
    panic("lexer: unknown token");
  }
}

static Token next(LexerState *state) {
  while (!lexer_is_end(state) && should_skip(lexer_peek(state)))
    lexer_consume(state);

  if (lexer_is_end(state))
    return payloadless_token(TokenEnd);

  char c = lexer_peek(state);

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

void free_tokens(Array *tokens) {
  /* Free the cloned idents */
  for (size_t i = 0; i < tokens->len; i++) {
    Token *token = array_get_ptr(tokens, i);
    if (token->kind == TokenIdent)
      free(token->ident);
  }

  destroy_array(tokens);
}
