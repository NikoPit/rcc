#include <spawn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define NOB_IMPLEMENTATION
#include "libs/nob.h"

#define STB_DS_IMPLEMENTATION
#include "libs/stb_ds.h"

/* Misc */

[[nodiscard]]
static bool string_equals(const char *lhs, const char *rhs) {
  return strcmp(lhs, rhs) == 0;
}

[[nodiscard]]
static bool ends_with(const char *str, const char *suffix) {
  auto len = strlen(str);
  auto suffix_len = strlen(suffix);

  return len >= suffix_len &&
         memcmp(str + len - suffix_len, suffix, suffix_len) == 0;
}

/* Command parsing */

typedef enum { Make, Run } Command;

[[nodiscard]]
static Command parse_cmd_string(char *cmd_string) {
  constexpr char usage[] = "Usage: ./make <cmd>";

  if (string_equals(cmd_string, "run")) {
    return Run;
  } else {
    fputs(usage, stderr);
    exit(EXIT_FAILURE);
  }
}

[[nodiscard]]
static Command parse_cmd(int argc, char *argv[]) {
  if (argc == 1 /* ./make with no args */) {
    return Make;
  } else /* Allow args more then 2 for cases like `make run -- hello.c` */ {
    return parse_cmd_string(argv[1]);
  }
}

/* Command impls */

static constexpr char output_path[] = "rcc";

[[nodiscard]]
static bool push_source(Nob_Walk_Entry entry) {
  char *** /* Pointer to array of strings */ sources = entry.data;
  if (entry.type == FILE_REGULAR && ends_with(entry.path, ".c"))
    arrput(*sources, strdup(entry.path));
  return true;
}

[[nodiscard]]
static const char ** /* Array of strings */ collect_sources(void) {
  constexpr char src_dir[] = "src/";

  const char **sources = nullptr;

  if (!nob_walk_dir(src_dir, push_source, .data = &sources)) {
    fputs("Failed to collect sources", stderr);
    exit(EXIT_FAILURE);
  }

  return sources;
}

static void make() {
  Cmd cmd = {0};

  nob_cc(&cmd);
  nob_cc_flags(&cmd);
  nob_cc_output(&cmd, output_path);
  nob_cmd_append(&cmd, "-std=gnu23");

  auto sources = collect_sources();
  for (ptrdiff_t i = 0; i < arrlen(sources); i++) {
    nob_cmd_append(&cmd, sources[i]);
  }

  if (!cmd_run(&cmd))
    exit(EXIT_FAILURE);

  for (ptrdiff_t i = 0; i < arrlen(sources); i++) {
    free((void *)sources[i]); /* Free the strdup'ed string */
  }
  arrfree(sources);
}

[[nodiscard]]
static int wait_status_to_code(int status) {
  if (WIFEXITED(status)) {
    return WEXITSTATUS(status);
  } else if (WIFSIGNALED(status)) {
    return 128 + WTERMSIG(status);
  } else {
    fputs("Unknown wait status", stderr);
    exit(EXIT_FAILURE);
  }
}

extern char **environ;
static void run(int main_argc, char *main_argv[]) {
  char *spawned_argv[256];
  auto spawned_argc = 0;

  spawned_argv[0] = output_path;
  spawned_argc++;

  if (main_argc >= 3) {
    if (string_equals(main_argv[2], "--")) {
      auto i = 3;
      while (i < main_argc) {
        if (spawned_argc >= 255) {
          fputs("make run: too many arguments", stderr);
          exit(EXIT_FAILURE);
        }

        spawned_argv[spawned_argc] = main_argv[i];
        spawned_argc++;
        i++;
      }
    } else {
      fputs("If you're trying to pass arguments to rcc, use ./make run -- "
            "<args> instead of ./make run <args>",
            stderr);
      exit(EXIT_FAILURE);
    }
  }

  spawned_argv[spawned_argc] = nullptr; /* Null terminator */
  spawned_argc++;

  make();

  pid_t pid;
  auto error =
      posix_spawn(&pid, output_path, nullptr, nullptr, spawned_argv, environ);
  if (error != 0) {
    fprintf(stderr, "make run: %s\n", strerror(error));
    exit(EXIT_FAILURE);
  }

  int status;
  if (waitpid(pid, &status, 0) == -1) {
    perror("make run");
    exit(EXIT_FAILURE);
  }
  exit(wait_status_to_code(status)); /* Carry over rcc's exit status */
}

/* Entry */

int main(int argc, char *argv[]) {
  auto command = parse_cmd(argc, argv);

  switch (command) {
  case Make:
    make();
    break;
  case Run:
    run(argc, argv);
    break;
  }

  return EXIT_SUCCESS;
}
