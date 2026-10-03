#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "utils/fs.h"
#include "utils/result.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    puts("Usage: rcc <file>");
    return EXIT_FAILURE;
  }

  auto file_content_res = read_file(argv[1]);

  if (file_content_res.kind == ResultErr) {
    fprintf(stderr, "reading input file: %s\n",
            strerror(file_content_res.err /* errno */));
    return EXIT_FAILURE;
  }

  auto file_content = file_content_res.ok;

  auto tokens = lex(file_content);
  free(file_content);

  return EXIT_SUCCESS;
}
