#include <stdio.h>
#include <stdlib.h>

#include "lexer/lexer.h"
#include "utils/fs.h"
#include "utils/result.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    puts("Usage: rcc <file>");
    return EXIT_FAILURE;
  }

  EXTRACT_OK(ReadFileResult, read_file(argv[1]), char *, content, {
    perror("reading input file");
    return EXIT_FAILURE;
  });

  Array tokens = lex(content);
  free(content);

  return EXIT_SUCCESS;
}
