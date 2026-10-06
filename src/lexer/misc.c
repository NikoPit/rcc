#include "misc.h"
#include "../diagnostic.h"
#include "core.h"

[[nodiscard]]
Token lex_misc(LexerState *state) {
  switch (lexer_consume(state)) {
  case '{':
    return payloadless_token(TokenLeftBrace);
  case '}':
    return payloadless_token(TokenRightBrace);
  case '(':
    return payloadless_token(TokenLeftParen);
  case ')':
    return payloadless_token(TokenRightParen);
  case ';':
    return payloadless_token(TokenSemicolon);
  default:
    auto span =
        (Span){.start = previous_pos(state) /* Already consumed */, .len = 1};
    diag_error(state->ctx, span, "Unknown token");
  }
}
