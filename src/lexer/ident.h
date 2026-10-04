#pragma once

#include "core.h"
#include "token.h"

[[nodiscard]]
Token lex_ident(LexerState *state);

/* Returns weather `c` could be the start of an ident. */
[[nodiscard]]
bool is_ident_start(char c);
