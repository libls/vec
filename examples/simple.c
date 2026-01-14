#include "ls_vec.h"

LS_VEC_INLINE(int, int_vec)

int main(void) {
    int_vec vec;
    int_vec_init(&vec);
    int_vec_push(&vec, 12);
    int_vec_clear(&vec);
    return 0;
}
