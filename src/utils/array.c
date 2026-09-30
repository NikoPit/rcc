#include "array.h"
#include "misc.h"

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

static void *array_slot_at(Array *array, size_t index) {
  return (char *)array->base + (index * array->element_size);
}

static void expand_array(Array *array) {
  array->capacity *= 2;

  void *new_base = realloc(array->base, array->capacity * array->element_size);
  if (new_base == NULL)
    panic("array: out of memory");

  array->base = new_base;
}

void array_set_ptr(Array *array, size_t index, void *value) {
  while (index >= array->capacity)
    expand_array(array);

  void *slot_addr = array_slot_at(array, index);
  memcpy(slot_addr, value, array->element_size);

  if (index >= array->len)
    array->len = index + 1;
}

void array_push_ptr(Array *array, void *value) {
  array_set_ptr(array, array->len, value);
}

void *array_get_ptr(Array *array, size_t index) {
  if (index >= array->len)
    panic("array: index out of bounds");

  return array_slot_at(array, index);
}

void destroy_array(Array *array) { free(array->base); }
