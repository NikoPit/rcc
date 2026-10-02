#ifndef RCC_LEXER_TOKEN_H
#define RCC_LEXER_TOKEN_H

typedef enum {
  TokenVoid,

  /* Types */
  TokenInt,

  TokenIdent,

  /* Statements */
  TokenReturn,

  /* Basic single characters */
  TokenLeftBrace,
  TokenRightBrace,
  TokenLeftParen,
  TokenRightParen,
  TokenSemicolon,

  TokenEnd, /* End of the source code */

  TokenReserved /* Shoudlen't be generated, should be used like null. */
} TokenKind;

typedef struct {
  TokenKind kind;

  union {
    char *ident;
  };
} Token;

#endif
