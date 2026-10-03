#ifndef RCC_FS_H
#define RCC_FS_H

#include "result.h"
#include "string.h"

DEF_RESULT(ReadFile, string, int /* errno */);

[[nodiscard]]
ReadFileResult read_file(const_string path);

#endif
