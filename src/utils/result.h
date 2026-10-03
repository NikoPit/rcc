#ifndef RCC_RESULT_H
#define RCC_RESULT_H

#include <stdint.h>

typedef enum { ResultOk, ResultErr } ResultKind;

typedef struct {
  uint8_t _;
} Empty;

#define MK_EMPTY ((Empty){0})

#define DEF_RESULT(name, ok_type, err_type)                                    \
  typedef struct {                                                             \
    ResultKind kind;                                                           \
    union {                                                                    \
      ok_type ok;                                                              \
      err_type err;                                                            \
    };                                                                         \
  } name##Result

#define EXTRACT_OK(result_expr, ok_var_name, err)                              \
  auto _rcc_tmp_result_##ok_var_name = result_expr;                            \
  if (_rcc_tmp_result_##ok_var_name.kind == ResultErr)                         \
    err auto ok_var_name = _rcc_tmp_result_##ok_var_name.ok;

#define RESULT_OK(type, value)                                                 \
  (type) { .kind = ResultOk, .ok = value }
#define RESULT_ERR(type, value)                                                \
  (type) { .kind = ResultErr, .err = value }

#endif
