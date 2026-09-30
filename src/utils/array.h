#ifndef RCC_UTILS_ARRAY_H
#define RCC_UTILS_ARRAY_H

#include "misc.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  void *base;
  size_t len; /* Amounts of element in this array. */
  size_t capacity;
  size_t element_size;
} Array;

#define create_array(type) create_array_with_size(sizeof(type))

Array create_array_with_size(size_t element_size);

#define array_set(array, index, value)                                         \
  array_set_ptr(array, index, (void *)&value)

void array_set_ptr(Array *array, size_t index, void *value);

#define array_push(array, type, value)                                         \
  do {                                                                         \
    type _rcc_array_push_tmp_value_ = value;                                   \
    /* Type that your trying to push doesn't match the type of this array. */  \
    if (sizeof(type) != (array)->element_size)                                 \
      panic("array: element type mismatch");                                   \
    array_push_ptr(array, &_rcc_array_push_tmp_value_);                        \
  } while (0)

void array_push_ptr(Array *array, void *value);

#define array_get(array, type, index) (*(type *)array_get_ptr(array, index))

void *array_get_ptr(Array *array, size_t index);

void destroy_array(Array *array);

#endif
