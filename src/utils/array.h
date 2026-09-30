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

/* Public wrapper of `__create_array_with_size`, taking a type instead of the
 * size of that type.
 *
 * Becuase you can not pass a type to a function, only a
 * macro can do that. */
#define create_array(type) create_array_with_size(sizeof(type))

/* Creates a new array with the size of `BASE_ARRAY_SIZE`, with `element_size`
 * as the size per element, in bytes.
 *
 * Private interface. Use `create_array` instead, which takes the type, instead
 * of the size of that type */
Array create_array_with_size(size_t element_size);

/* Public wrapper of `__array_set`, taking the value it self instead of a
 * pointer to that value. */
#define array_set(array, index, value)                                         \
  array_set_inner(array, index, (void *)&value)

/* Sets `index` of `array` as `value`.
 *
 * Private interface. Use `array_set` instead. */
void array_set_inner(Array *array, size_t index, void *value);

#define array_push(array, type, value)                                         \
  do {                                                                         \
    type _rcc_array_push_tmp_value_ = value;                                   \
    /* Type that your trying to push doesn't match the type of this array. */  \
    if (sizeof(type) != (array)->element_size)                                 \
      panic("array: element type mismatch");                                   \
    array_push_inner(array, &_rcc_array_push_tmp_value_);                      \
  } while (0)

/* Pushes `value` to the end of `array`. */
void array_push_inner(Array *array, void *value);

#define array_get(array, type, index) (*(type *)array_get_inner(array, index))

/* Returns the pointer to `index` in `array`. */
void *array_get_inner(Array *array, size_t index);

void destroy_array(Array *array);

#endif
