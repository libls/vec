/* Lion's Standard (LS) type-safe ANSI C vector.
 *
 * Version: 2.0
 * Website: https://libls.org
 * Repo: https://github.com/libls/vec
 * SPDX-License-Identifier: MIT
 *
 * ==== TABLE OF CONTENTS ====
 *
 * 1. DESCRIPTION
 * 2. HOW TO USE
 * 3. LICENSE
 *
 * ==== 1. DESCRIPTION ====
 *
 * A minimal, terse, generic (macro code generated) header-only library in ANSI
 * C, which implements a vector (dynamically sized array).
 *
 * The implementation uses standard malloc/realloc for memory management and
 * automatically grows the capacity as needed. The memory allocator is
 * configurable.
 *
 * ==== 2. HOW TO USE ====
 *
 * Dynamically sized, type-safe vector.
 *
 * Use LS_VEC_INLINE to generate a static inline version of the library.
 * This is the "default" behavior.
 *
 * If you need a declaration and implementation separately, use
 * LS_VEC_DECL and make sure to call it with the same arguments as
 * LS_VEC_IMPL. Put LS_VEC_DECL in a header, and LS_VEC_IMPL in exactly
 * ONE source file.
 *
 * Simple example:
 *
 *     LS_VEC_INLINE(int, int_vector)
 *
 *     // somewhere in the same file
 *     int_vector vec;
 *     int_vector_init(&vec);
 *     int_vector_push(&vec, 42);
 *     // use vec.data, vec.size, etc.
 *     int_vector_free(&vec);
 *
 * Alternative example with decl and impl split:
 *
 *     // In your header file:
 *     LS_VEC_DECL(int, int_vector)
 *
 *     // In one source file:
 *     LS_VEC_IMPL(int, int_vector)
 *
 *     // Usage in your code:
 *     int_vector vec;
 *     int_vector_init(&vec);
 *     if (!int_vector_push(&vec, 42)) {
 *         // handle allocation failure
 *     }
 *     // access elements via vec.data[i]
 *     int_vector_free(&vec);
 *
 * You can configure a custom memory allocator by defining the macros LS_REALLOC
 * and LS_FREE globally. These are the only allocation functions required, and
 * they are expected to behave exactly as the standard requires. For example,
 * LS_FREE(NULL) must be valid, LS_REALLOC can fail, LS_REALLOC with NULL will
 * act like malloc, etc.
 *
 * ==== 3. LICENSE ====
 *
 * This file is provided under the MIT license. For commercial support and
 * maintenance, feel free to use the e-mail below to contact the author(s).
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2026 Lion Kortlepel <libls@kortlepel.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef LS_REALLOC
#define LS_REALLOC realloc
#endif

#ifndef LS_FREE
#define LS_FREE free
#endif

#define LS_VEC_DECL(T, name)                                                   \
    typedef struct name {                                                      \
        size_t size;                                                           \
        size_t capacity;                                                       \
        T* data;                                                               \
    } name;                                                                    \
    void name##_init(name* vec);                                               \
    void name##_free(name* vec);                                               \
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
    specifier void name##_free(name* vec) {                                    \
        if (vec->data) {                                                       \
            LS_FREE(vec->data);                                                \
            vec->data = NULL;                                                  \
        }                                                                      \
        vec->size = 0;                                                         \
        vec->capacity = 0;                                                     \
    }                                                                          \
    specifier int name##_reserve(name* vec, size_t count) {                    \
        if (vec->capacity < count) {                                           \
            T* new_data;                                                       \
            size_t max_items = SIZE_MAX / sizeof(T);                           \
            size_t new_cap = vec->capacity + vec->capacity / 2 + 8;            \
            if (new_cap < count) {                                             \
                new_cap = count;                                               \
            }                                                                  \
            /* overflow check */                                               \
            if (new_cap > max_items) {                                         \
                return 0;                                                      \
            }                                                                  \
            new_data = (T*)LS_REALLOC(vec->data, new_cap * sizeof(T));         \
            if (new_data == NULL) {                                            \
                return 0;                                                      \
            }                                                                  \
            vec->capacity = new_cap;                                           \
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
    }                                                                          \
    specifier int name##_pop(name* vec, T* out_value) {                        \
        if (vec->size == 0) {                                                  \
            return 0;                                                          \
        }                                                                      \
        *out_value = vec->data[--vec->size];                                   \
        return 1;                                                              \
    }
