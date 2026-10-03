#ifndef RCC_LEXER_TOKEN_H
#define RCC_LEXER_TOKEN_H

#include "../utils/string.h"

typedef enum {
  TokenVoid,

  /* Types */
  TokenInt,

  TokenIdent,
  TokenNumber,

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
    string ident;
    long long number;
  };
} Token;

#endif
