#ifndef RCC_UTILS_STRING_H
#define RCC_UTILS_STRING_H

#include <string.h>

[[nodiscard]]
bool string_equals(const char *lhs, const char *rhs);

[[nodiscard]]
char *clone_string(const char *string);

#endif
