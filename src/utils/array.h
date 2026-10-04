#ifndef RCC_UTILS_ARRAY_H
#define RCC_UTILS_ARRAY_H

/* The type of an `stb_ds` array made up of `element_type`.
 *
 * Makes array types more explicit, to avoid confusion betwean normal pointer
 * types. */
#define array_t(element_type) element_type *

/* Creates an `stb_ds` array made out of `element_type`.
 *
 * Makes creating new arrays more explicit and allows usage of `auto`. */
#define create_array(element_type) (array_t(element_type)) nullptr

#endif
