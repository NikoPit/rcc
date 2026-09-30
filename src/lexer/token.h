#ifndef RCC_LEXER_TOKEN_H
#define RCC_LEXER_TOKEN_H

typedef enum {
  TokenVoid,

  TokenInt,

  TokenIdent,

  TokenReturn,

  TokenLeftBrace,
  TokenRightBrace,
  TokenLeftParen,
  TokenRightParen,

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
