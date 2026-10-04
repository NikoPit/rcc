#ifndef RCC_UTILS_STRING_H
#define RCC_UTILS_STRING_H

#include <string.h>

typedef char *string;
typedef const char *const_string;

[[nodiscard]]
bool string_equals(const_string lhs, const_string rhs);

#endif
