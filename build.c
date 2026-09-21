#include <stdio.h>
#include <stdlib.h>

#define NOB_IMPLEMENTATION
#include "libs/nob.h"

#define BUILD_DIR "build/"
#define SRC_DIR "src/"

static void fail(char *message) {
  puts(message);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  GO_REBUILD_URSELF(argc, argv);

  if (!mkdir_if_not_exists(BUILD_DIR))
    fail("Failed to create build directory");

  Cmd cmd = {0};

  nob_cc(&cmd);
  nob_cc_output(&cmd, BUILD_DIR "rcc");

  nob_cc_inputs(&cmd, SRC_DIR "main.c");

  if (!cmd_run(&cmd))
    fail("Failed to execute build command");

  return EXIT_SUCCESS;
}
