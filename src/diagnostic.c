#include "diagnostic.h"
#include "misc.h"
#include "utils/math.h"
#include "utils/misc.h"
#include "utils/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { DiagnosticLevelError, DiagnosticLevelWarning } DiagnosticLevel;

[[nodiscard]]
static const_string diag_level_str(DiagnosticLevel level) {
  switch (level) {
  case DiagnosticLevelError:
    return "error";
  case DiagnosticLevelWarning:
    return "warning";
  }

  panic("Unreachable");
}

[[nodiscard]]
static int column_number_at(const CompileContext *ctx, size_t offset) {
  auto column_number = 1;
  auto limit = min(offset, ctx->source_len);

  for (auto i = (size_t)0; i < limit; i++) {
    if (ctx->source[i] == '\n')
      column_number = 1;
    else
      column_number++;
  }

  return column_number;
}

[[nodiscard]]
static int line_number_at(const CompileContext *ctx, size_t offset) {
  auto line_number = 1;
  auto limit = min(offset, ctx->source_len);

  for (auto i = (size_t)0; i < limit; i++) {
    if (ctx->source[i] == '\n')
      line_number++;
  }

  return line_number;
}

static void print_line(const CompileContext *ctx, int line_number) {
  auto line_str = (char *)malloc(ctx->source_len + 1);
  auto line_str_size = 0;

  auto current_line_number = 1;
  for (auto i = (size_t)0;
       i < ctx->source_len && current_line_number <= line_number; i++) {
    if (current_line_number == line_number) {
      line_str[line_str_size] = ctx->source[i];
      line_str_size++;
    }

    if (ctx->source[i] == '\n')
      current_line_number++;
  }

  line_str[line_str_size] = '\0';
  line_str_size++;

  fputs(line_str, stderr);
  fputc('\n', stderr);

  free(line_str);
}

static void print_carets(int start_column, size_t len) {
  for (auto i = 0; i + 1 < start_column; i++)
    fputc(' ', stderr);
  for (auto i = (size_t)0; i < len; i++)
    fputc('^', stderr);
  fputc('\n', stderr);
}

static void print_diagnostic(const CompileContext *ctx, DiagnosticLevel level,
                             Span span, const_string message) {
  auto line_number = line_number_at(ctx, span.start);
  auto column_number = column_number_at(ctx, span.start);

  fprintf(stderr, "%s:%i:%i %s: %s\n", ctx->file_name, line_number,
          column_number, diag_level_str(level), message);
  print_line(ctx, line_number);
  print_carets(column_number, span.len);
}

[[noreturn]]
void diag_error(const CompileContext *ctx, Span span, const_string message) {
  print_diagnostic(ctx, DiagnosticLevelError, span, message);
  exit(EXIT_FAILURE);
}

void diag_warning(const CompileContext *ctx, Span span, const_string message) {
  print_diagnostic(ctx, DiagnosticLevelWarning, span, message);
}
