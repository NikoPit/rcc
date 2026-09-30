#include "fs.h"
#include "result.h"
#include <stdio.h>
#include <stdlib.h>

DEF_RESULT(FileSize, long, Empty);
static FileSizeResult file_size(FILE *file) {
  if (fseek(file, 0, SEEK_END) != 0)
    return RESULT_ERR(FileSizeResult, MK_EMPTY);

  long size = ftell(file);
  if (size < 0)
    return RESULT_ERR(FileSizeResult, MK_EMPTY);

  rewind(file);

  return RESULT_OK(FileSizeResult, size);
}

/* Reads the entire file at `path`. */
ReadFileResult read_file(char *path) {
  FILE *file = fopen(path, "rb");
  char *content = NULL;

  if (file == NULL)
    goto err;

  EXTRACT_OK(FileSizeResult, file_size(file), long, size, { goto err; });

  content = malloc(size + 1); /* +1 for the null terminator */

  if (content == NULL)
    goto err;

  if (fread(content, 1, size, file) != (size_t)size)
    goto err;

  content[size] = '\0';

  fclose(file);
  return RESULT_OK(ReadFileResult, content);

err:
  if (file != NULL)
    fclose(file);

  if (content != NULL)
    free(content);

  return RESULT_ERR(ReadFileResult, MK_EMPTY);
}
