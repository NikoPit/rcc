#include "fs.h"
#include "result.h"
#include "string.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

DEF_RESULT(FileSize, long, Empty);

[[nodiscard]]
static FileSizeResult file_size(FILE *file) {
  if (fseek(file, 0, SEEK_END) != 0)
    return RESULT_ERR(FileSizeResult, MK_EMPTY);

  auto size = ftell(file);
  if (size < 0)
    return RESULT_ERR(FileSizeResult, MK_EMPTY);

  rewind(file);

  return RESULT_OK(FileSizeResult, size);
}

/* Reads the entire file at `path`. */
[[nodiscard]]
ReadFileResult read_file(const_string path) {
  auto file = fopen(path, "rb");
  auto content = (char *)nullptr;

  if (file == nullptr)
    goto err;

  EXTRACT_OK(file_size(file), size, { goto err; });

  content = malloc(size + 1); /* +1 for the null terminator */

  if (content == nullptr)
    goto err;

  if (fread(content, 1, size, file) != (size_t)size)
    goto err;

  content[size] = '\0';

  fclose(file);
  return RESULT_OK(ReadFileResult, content);

err:
  if (file != nullptr)
    fclose(file);

  if (content != nullptr)
    free(content);

  return RESULT_ERR(ReadFileResult, errno);
}
