#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Lion's Standard (LS) ANSI C vector library.
 *
 * This is a vector library, i.e. a dynamically sized array.
 * To get started, you have to declare your type:
 *
 *      VEC(int)
 *
 * which creates a struct and the associated functions for a vec_int,
 * meaning a vector of type `int`. The resulting struct is named `vec_int`
 * and all associated functions are prefixed with that same name, for
 * example `vec_int_init`, `vec_int_push`, etc.
 *
 * You can choose a name for long or complex types like this:
 *
 *      VEC_NAMED(int*, intp)
 *
 * which results in vec_intp, vec_intp_push, etc. or
 *
 *      VEC_NAMED(struct Car, car)
 *
 * which results in vec_car, vec_car_push, etc.
 *
 * The VEC and VEC_NAMED macros create both the declaration and implementation.
 * If you'd like to split that, call
 *
 *      VEC_DECL(int)
 *
 * to generate the declarations, and somewhere else
 *
 *      VEC_IMPL(int)
 *
 * to generate the implementation. The same with VEC_NAMED_DECL
 * and VEC_NAMED_IMPL.
 *
 * Full example:
 *
 *      VEC(int)
 *
 *      int main(void) {
 *          vec_int vec;
 *          vec_int_init(&vec);
 *          for (int i = 0; i < 10; ++i) {
 *              vec_int_push(&vec, i);
 *          }
 *          vec_int_clear(&vec);
 *      }
 *
 */

#define LS_VEC_DECL(T, name)                                                   \
    typedef struct name {                                                      \
        size_t size;                                                           \
        size_t capacity;                                                       \
        T* data;                                                               \
    } name;                                                                    \
    void name##_init(name* vec);                                               \
    void name##_clear(name* vec);                                              \
    int name##_reserve(name* vec, size_t count);                               \
    int name##_push(name* vec, T value);
#define LS_VEC_IMPL(T, name) _ls_VEC_IMPL_DETAIL(T, name, )

#define LS_VEC_INLINE(T, name)                                                 \
    typedef struct name {                                                      \
        size_t size;                                                           \
        size_t capacity;                                                       \
        T* data;                                                               \
    } name;                                                                    \
    _ls_VEC_IMPL_DETAIL(T, name, static)

#define _ls_VEC_IMPL_DETAIL(T, name, specifier)                                \
    specifier void name##_init(name* vec) { memset(vec, 0, sizeof(*vec)); }    \
    specifier void name##_clear(name* vec) {                                   \
        if (vec->data) {                                                       \
            free(vec->data);                                                   \
            vec->data = NULL;                                                  \
        }                                                                      \
        vec->size = 0;                                                         \
        vec->capacity = 0;                                                     \
    }                                                                          \
    specifier int name##_reserve(name* vec, size_t count) {                    \
        if (vec->capacity < count) {                                           \
            T* new_data;                                                       \
            size_t total;                                                      \
            if (count == 0) {                                                  \
                vec->capacity = 5;                                             \
            } else {                                                           \
                size_t new_cap = (size_t)((float)vec->capacity * 1.6f + 1.0f); \
                vec->capacity = new_cap > count ? new_cap : count;             \
            }                                                                  \
            total = vec->capacity * sizeof(T);                                 \
            if (vec->capacity != 0 && total / vec->capacity != sizeof(T))      \
                return 0; /* integer overflow */                               \
            new_data = (T*)realloc(vec->data, total);                          \
            if (!new_data)                                                     \
                return 0;                                                      \
            vec->data = new_data;                                              \
        }                                                                      \
        return 1;                                                              \
    }                                                                          \
    specifier int name##_push(name* vec, T value) {                            \
        if (!name##_reserve(vec, vec->size + 1)) {                             \
            return 0;                                                          \
        }                                                                      \
        vec->data[vec->size++] = value;                                        \
        return 1;                                                              \
    }
