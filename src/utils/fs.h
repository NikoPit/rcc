#pragma once

#include "result.h"
#include "string.h"

DEF_RESULT(ReadFile, string, int /* errno */);

[[nodiscard]]
ReadFileResult read_file(const_string path);
