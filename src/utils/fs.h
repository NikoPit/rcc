#ifndef RCC_FS_H
#define RCC_FS_H

#include "result.h"

DEF_RESULT(ReadFile, char *, Empty);

ReadFileResult read_file(const char *path);

#endif
