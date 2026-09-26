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
  FILE *file = fopen(path, "r");

  if (file == NULL)
    return RESULT_ERR(ReadFileResult, MK_EMPTY);

  EXTRACT_OK(FileSizeResult, file_size(file), long, size, {
    fclose(file);
    return RESULT_ERR(ReadFileResult, MK_EMPTY);
  });

  char *content = malloc(size + 1); /* +1 for the null terminator */
  if (content == NULL) {
    fclose(file);
    return RESULT_ERR(ReadFileResult, MK_EMPTY);
  }

  if (fread(content, 1, size, file) != size) {
    free(content);
    fclose(file);
    return RESULT_ERR(ReadFileResult, MK_EMPTY);
  }

  content[size] = '\0';

  fclose(file);
  return RESULT_OK(ReadFileResult, content);
}
