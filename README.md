# LS Vector

Minimal, single-header dynamic array (vector) for C.

- Type-safe via macro code generation (not macro function calls)
- ANSI C
- Header-only
- Dynamic resizing with configurable growth factor
- Configurable memory allocators
- Full memory management with overflow protection

## Quick Start

1. Copy `ls_vec.h` to your project.
2. Define any number of vector types:
    ```c
    LS_VEC_INLINE(int, int_vector)
    ```

    **OR** for separate declaration/implementation:

    ```c
    // In header:
    LS_VEC_DECL(int, int_vector)

    // In source file:
    LS_VEC_IMPL(int, int_vector)
    ```
3. Use the vector with normal C function calls:
    ```c
    int_vector vec;
    int_vector_init(&vec);
    if (!int_vector_push(&vec, 42)) {
        // push failed, allocation failure, handle that here
    }
    // Access elements via vec.data[i]
    // Check size via vec.size
    int_vector_clear(&vec);
    ```

See [`ls_vec.h`](ls_vec.h) for detailed documentation and usage patterns.

## Custom Memory Allocators

Configure custom allocators by defining these macros before including the header:

```c
#define LS_REALLOC my_realloc
#define LS_FREE my_free
#include "ls_vec.h"
```

The allocators must behave exactly like standard `realloc` and `free`.

See [`ls_vec.h`](ls_vec.h) for detailed documentation and usage patterns.

## License

MIT.
