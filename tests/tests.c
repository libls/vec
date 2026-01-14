#define LS_TEST_IMPLEMENTATION
#include "ls_test.h"
#include "../ls_vec.h"

LS_VEC_INLINE(int, vec_int)

TEST_CASE(vec_init_clear) {
    vec_int v;
    vec_int_init(&v);
    ASSERT_EQ(v.size, 0, "%zu");
    ASSERT_EQ(v.capacity, 0, "%zu");
    ASSERT_EQ(v.data, NULL, "%p");
    vec_int_clear(&v);
    ASSERT_EQ(v.size, 0, "%zu");
    ASSERT_EQ(v.capacity, 0, "%zu");
    ASSERT_EQ(v.data, NULL, "%p");
    return 0;
}

TEST_CASE(vec_push_and_access) {
    int i;
    vec_int v;
    vec_int_init(&v);
    for (i = 0; i < 10; ++i)
        ASSERT_EQ(vec_int_push(&v, i), 1, "%d");
    ASSERT_EQ(v.size, 10, "%zu");
    for (i = 0; i < 10; ++i)
        ASSERT_EQ(v.data[i], i, "%d");
    vec_int_clear(&v);
    return 0;
}

TEST_CASE(vec_reserve_grow) {
    vec_int v;
    vec_int_init(&v);
    ASSERT_EQ(vec_int_reserve(&v, 20), 1, "%d");
    ASSERT_GE(v.capacity, 20, "%zu");
    vec_int_clear(&v);
    return 0;
}

TEST_CASE(vec_push_overflow) {
    vec_int v;
    vec_int_init(&v);
    v.capacity = (size_t)-1 / sizeof(int); // force next reserve to overflow
    v.size = v.capacity;
    int ret = vec_int_push(&v, 123);
    ASSERT_EQ(ret, 0, "%d");
    vec_int_clear(&v);
    return 0;
}

TEST_MAIN
