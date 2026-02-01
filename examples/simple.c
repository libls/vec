#include "ls_vec.h"

LS_VEC_INLINE(int, int_vec)

int main(void) {
    int v;
    int_vec vec;
    int_vec_init(&vec);
    int_vec_push(&vec, 12);
    int_vec_pop(&vec, &v);
    int_vec_free(&vec);
    return 0;
}
