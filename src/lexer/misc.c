#include "misc.h"
#include "../utils/misc.h"

[[nodiscard]]
Token lex_misc(LexerState *state) {
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
