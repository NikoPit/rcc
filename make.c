#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L &&                \
    !defined(                                                                  \
        __STRICT_ANSI__) /* -std=c23, aka non GNU C, defines __STRICT_ANSI__,  \
                            so here it must not define __STRICT_ANSI__ */

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

#include "libs/json.h"

/* Misc */

/* The type of an `stb_ds` array made up of `element_type`.
 *
 * Makes array types more explicit, to avoid confusion betwean normal pointer
 * types. */
#define array_t(element_type) element_type *

/* Creates an `stb_ds` array made out of `element_type`.
 *
 * Makes creating new arrays more explicit and allows usage of `auto`. */
#define create_array(element_type) (array_t(element_type)) nullptr

#define new_zeroed(type)                                                       \
  (type) {}

typedef char *string;
typedef const char *const_string;
typedef char constexpr_string[];

[[nodiscard]]
static bool string_equals(const_string lhs, const_string rhs) {
  return strcmp(lhs, rhs) == 0;
}

[[nodiscard]]
static bool ends_with(const_string str, const_string suffix) {
  auto len = strlen(str);
  auto suffix_len = strlen(suffix);

  return len >= suffix_len &&
         memcmp(str + len - suffix_len, suffix, suffix_len) == 0;
}

[[nodiscard]]
static string working_directory() {
  auto cwd = new_zeroed(char[PATH_MAX]);

  if (getcwd(cwd, sizeof(cwd)) == nullptr) {
    perror("make");
    exit(EXIT_FAILURE);
  }

  return strdup(cwd);
}

static void write_file(const_string content, const_string path) {
  auto file = fopen(path, "wb");

  if (file == nullptr) {
    perror("make");
    exit(EXIT_FAILURE);
  }

  if (fputs(content, file) == EOF) {
    perror("make");
    exit(EXIT_FAILURE);
  }

  if (fclose(file) != 0) {
    perror("make");
    exit(EXIT_FAILURE);
  }
}

/* Free an array made out of strings. Strdup'd strings need to be freed
 * manually. */
static void free_string_array(const array_t(string) array) {
  for (auto i = (ptrdiff_t)0; i < arrlen(array); i++) {
    free(array[i]);
  }

  arrfree(array);
}

/* Command parsing */

typedef enum { Make, Run, CompileCommands } Command;

[[nodiscard]]
static Command parse_cmd_string(string cmd_string) {
  constexpr constexpr_string usage = "Usage: ./make <cmd>";

  if (string_equals(cmd_string, "run")) {
    return Run;
  } else if (string_equals(cmd_string, "compile_commands")) {
    return CompileCommands;
  } else {
    fputs(usage, stderr);
    exit(EXIT_FAILURE);
  }
}

[[nodiscard]]
static Command parse_cmd(int argc, string argv[]) {
  if (argc == 1 /* ./make with no args */) {
    return Make;
  } else /* Allow args more then 2 for cases like `make run -- hello.c` */ {
    return parse_cmd_string(argv[1]);
  }
}

/* Command impls */

static constexpr constexpr_string output_path = "rcc";

[[nodiscard]]
static bool push_source(Nob_Walk_Entry entry) {
  auto sources = (array_t(string) *)entry.data;
  if (entry.type == FILE_REGULAR && ends_with(entry.path, ".c"))
    arrput(*sources, strdup(entry.path));
  return true;
}

[[nodiscard]] static const array_t(string) collect_sources() {
  constexpr constexpr_string src_dir = "src/";

  auto sources = create_array(string);

  if (!nob_walk_dir(src_dir, push_source, .data = &sources)) {
    fputs("Failed to collect sources", stderr);
    exit(EXIT_FAILURE);
  }

  return sources;
}

static void append_base_flags(Cmd *cmd) {
  nob_cc(cmd);
  nob_cc_flags(cmd);
  nob_cc_output(cmd, output_path);
  nob_cmd_append(cmd, "-std=gnu23");
}

static void make() {
  auto cmd = (Cmd){0};

  append_base_flags(&cmd);

  auto sources = collect_sources();
  for (auto i = (ptrdiff_t)0; i < arrlen(sources); i++) {
    nob_cmd_append(&cmd, sources[i]);
  }

  if (!cmd_run(&cmd))
    exit(EXIT_FAILURE);

  free_string_array(sources);
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

extern string *environ;
static void run(int main_argc, string main_argv[]) {
  auto spawned_argv = new_zeroed(string[256]);
  auto spawned_argc = 0;

  auto duped_output_path = strdup(output_path);

  if (duped_output_path == nullptr) {
    perror("make run");
    goto err;
  }

  spawned_argv[0] = duped_output_path;
  spawned_argc++;

  if (main_argc >= 3) {
    if (string_equals(main_argv[2], "--")) {
      auto i = 3;
      while (i < main_argc) {
        if (spawned_argc >= 255) {
          fputs("make run: too many arguments", stderr);
          goto err;
        }

        spawned_argv[spawned_argc] = main_argv[i];
        spawned_argc++;
        i++;
      }
    } else {
      fputs("If you're trying to pass arguments to rcc, use ./make run -- "
            "<args> instead of ./make run <args>",
            stderr);
      goto err;
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
    goto err;
  }

  int status;
  if (waitpid(pid, &status, 0) == -1) {
    perror("make run");
    goto err;
  }

  free(duped_output_path);
  exit(wait_status_to_code(status)); /* Carry over rcc's exit status */

err:
  if (duped_output_path != nullptr)
    free(duped_output_path);

  exit(EXIT_FAILURE);
}

static void compile_commands() {
  auto sources = collect_sources();
  auto json = json_create_array(); /* root */

  auto cwd = working_directory();

  for (auto i = (ptrdiff_t)0; i < arrlen(sources); i++) {
    auto entry = json_create_object();

    json_object_set(entry, "file", json_create_string(sources[i]));
    json_object_set(entry, "output", json_create_string(output_path));
    json_object_set(entry, "directory", json_create_string(cwd));

    auto cmd = (Cmd){0};
    append_base_flags(&cmd);
    cmd_append(&cmd, sources[i]);

    auto arguments = json_create_array();
    for (size_t j = 0; j < cmd.count; j++) {
      json_array_append(arguments, json_create_string(cmd.items[j]));
    }
    json_object_set(entry, "arguments", arguments);

    json_array_append(json, entry);

    cmd_free(cmd);
  }

  constexpr constexpr_string compile_commands_path = "compile_commands.json";
  auto compile_commands = json_serialize(json, false);
  write_file(compile_commands, compile_commands_path);

  free(cwd);
  free_string_array(sources);
  free(compile_commands);
  json_free(json);
}

/* Entry */

int main(int argc, string argv[]) {
  auto command = parse_cmd(argc, argv);

  switch (command) {
  case Make:
    make();
    break;
  case Run:
    run(argc, argv);
    break;
  case CompileCommands:
    compile_commands();
    break;
  }

  return EXIT_SUCCESS;
}

#else

#include <stdio.h>
#include <stdlib.h>

int main(void) {
  fputs(
      "make.c requires GNU C23 to run. Please compile make.c with -std=gnu23.",
      stderr);
  return EXIT_FAILURE;
}

#endif
