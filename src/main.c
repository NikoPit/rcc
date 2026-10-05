#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "misc.h"
#include "utils/fs.h"
#include "utils/result.h"
#include "utils/string.h"

int main(int argc, string argv[]) {
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

  auto ctx = (CompileContext){.source = file_content,
                              .source_len = strlen(file_content),
                              .file_name = argv[1]};

  auto tokens = lex(&ctx);
  free(file_content);

  (void)tokens;

  return EXIT_SUCCESS;
}
