#include "lexer.h"
#include "utils/bool.h"
#include "utils/misc.h"

typedef struct {
  const char *code;
  Array tokens;
  size_t pos;
} LexerState;

#define MK_PAYLOAD(type, value) {.type = value}

/* Returns the next character in the code; returns `NULL` if theres nothing
 * left. */
static const char *peek(LexerState *state) {
  if (state->pos >= strlen(state->code)) {
    return NULL;
  }

  return &state->code[state->pos];
}

/* Advance the lexer and returns the next character. */
static char consume(LexerState *state) { return state->code[state->pos++]; }

static void consume_and_push(LexerState *state, Token token) {
  consume(state);
  array_push(&state->tokens, Token, token);
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

/* Constructs a `Token` from a `TokenKind` without a payload. */
static Token payloadless_token(TokenKind kind) {
  Token token = {.kind = kind, .payload = {}};

  return token;
}

static void expect(LexerState *state, char *text) {
  for (char *c = text; *c != '\0'; c++) {
    if (consume(state) != *c)
      fail("lexer: expect: consumed text didn't match expected");
  }
}

#define IDENT_BUFFER_SIZE 256
static void lex_ident(LexerState *state) {
  char ident[IDENT_BUFFER_SIZE] = {};
  size_t size = 0;

  while (peek(state) != NULL && is_ident_continue(*peek(state))) {
    ident[size] = consume(state);
    size++;

    if (size >= IDENT_BUFFER_SIZE)
      fail("lexer: lex_ident: ident size exceeded max size (256)");
  }

  ident[size] = '\0';

  Token token = {.kind = TokenIdent,
                 .payload = MK_PAYLOAD(ident, strdup(ident))};

  array_push(&state->tokens, Token, token);
}

Array lex(const char *code) {
  LexerState state = {0};
  state.code = code;
  state.tokens = create_array(Token);

  while (peek(&state) != NULL) {
    char next = *peek(&state);

    switch (next) {
    case '{':
      consume_and_push(&state, payloadless_token(TokenLeftBrace));
      break;
    case '}':
      consume_and_push(&state, payloadless_token(TokenRightBrace));
      break;
    case '(':
      consume_and_push(&state, payloadless_token(TokenLeftParen));
      break;
    case ')':
      consume_and_push(&state, payloadless_token(TokenRightParen));
      break;

    case 'v':
      expect(&state, "void");
      array_push(&state.tokens, Token, payloadless_token(TokenVoid));
      break;
    case 'i':
      expect(&state, "int");
      array_push(&state.tokens, Token, payloadless_token(TokenInt));
      break;
    case 'r':
      expect(&state, "return");
      array_push(&state.tokens, Token, payloadless_token(TokenReturn));
      break;

    case ' ':
    case '\n':
      consume(&state);
      break;

    default:
      if (is_ident_start(next)) {
        lex_ident(&state);
      } else {
        fail("lexer: unknown token");
      }
    }
  }

  return state.tokens;
}
