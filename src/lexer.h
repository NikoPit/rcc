#ifndef RCC_LEXER_H
#define RCC_LEXER_H

#include "utils/array.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  TokenVoid,

  TokenInt,

  TokenIdent,

  TokenReturn,

  TokenLeftBrace,
  TokenRightBrace,
  TokenLeftParen,
  TokenRightParen,
} TokenKind;

typedef struct {
  TokenKind kind;

  union {
    char *ident;
  } payload;
} Token;

Array lex(const char *code);

#endif
