#include "array.h"
#include "misc.h"

/* Creates a new array with the size of `BASE_ARRAY_SIZE`, with `element_size`
 * as the size per element, in bytes.
 *
 * Private interface. Use `create_array` instead, which takes the type, instead
 * of the size of that type */
#define BASE_ARRAY_SIZE 16
Array create_array_with_size(size_t element_size) {
  void *base = malloc(element_size * BASE_ARRAY_SIZE);

  if (base == NULL)
    panic("array: out of memory");

  Array array = {.base = base,
                 .len = 0,
                 .capacity = BASE_ARRAY_SIZE,
                 .element_size = element_size};

  return array;
}

/* Returns the address of `index` in `array`. */
static void *array_slot_at(Array *array, size_t index) {
  return (char *)array->base + (index * array->element_size);
}

/* Doubles the capacity of `array` and allocates memory for the new capcity.
 *
 * Private interface, do not use. Using `array_set` / `array_push` will
 * automatically expand the array if the capacity is not enough. */
static void expand_array(Array *array) {
  array->capacity *= 2;

  void *new_base = realloc(array->base, array->capacity * array->element_size);
  if (new_base == NULL)
    panic("array: out of memory");

  array->base = new_base;
}

/* Sets `index` of `array` as `value`.
 *
 * Private interface. Use `array_set` instead. */
void array_set_inner(Array *array, size_t index, void *value) {
  while (index >= array->capacity)
    expand_array(array);

  void *slot_addr = array_slot_at(array, index);
  memcpy(slot_addr, value, array->element_size);

  if (index >= array->len)
    array->len = index + 1;
}

/* Pushes `value` to the end of `array`. */
void array_push_inner(Array *array, void *value) {
  array_set_inner(array, array->len, value);
}

/* Returns the pointer to `index` in `array`. */
void *array_get_inner(Array *array, size_t index) {
  if (index >= array->len)
    panic("array: index out of bounds");

  return array_slot_at(array, index);
}

void destroy_array(Array *array) { free(array->base); }
