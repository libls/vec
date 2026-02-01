#define LS_TEST_IMPLEMENTATION
#include "../ls_vec.h"
#include "ls_test.h"

LS_VEC_INLINE(int, vec_int)

TEST_CASE(vec_init_free) {
    vec_int v;
    vec_int_init(&v);
    ASSERT_EQ(v.size, 0, "%zu");
    ASSERT_EQ(v.capacity, 0, "%zu");
    ASSERT_EQ(v.data, NULL, "%p");
    vec_int_free(&v);
    ASSERT_EQ(v.size, 0, "%zu");
    ASSERT_EQ(v.capacity, 0, "%zu");
    ASSERT_EQ(v.data, NULL, "%p");
    return 0;
}

TEST_CASE(vec_push_pop_1000) {
    int i;
    vec_int v;
    vec_int_init(&v);

    // Push 1000 items
    for (i = 0; i < 1000; ++i) {
        ASSERT_EQ(vec_int_push(&v, i), 1, "%d");
    }
    ASSERT_EQ(v.size, 1000, "%zu");

    // Pop 1000 items and check values
    for (i = 999; i >= 0; --i) {
        int val;
        ASSERT_EQ(vec_int_pop(&v, &val), 1, "%d");
        ASSERT_EQ(val, i, "%d");
    }
    ASSERT_EQ(v.size, 0, "%zu");

    vec_int_free(&v);
    return 0;
}

TEST_CASE(vec_reserve) {
    vec_int v;
    vec_int_init(&v);
    ASSERT_EQ(vec_int_reserve(&v, 10), 1, "%d");
    ASSERT_EQ(v.capacity, 10, "%zu");
    vec_int_free(&v);
    return 0;
}

TEST_CASE(vec_free) {
    vec_int v;
    int i = 0;
    vec_int_init(&v);
    for (i = 0; i < 100; ++i) {
        vec_int_push(&v, i);
    }
    vec_int_free(&v);
    ASSERT_EQ(v.size, 0, "%zu");
    ASSERT_EQ(v.capacity, 0, "%zu");
    ASSERT_EQ(v.data, NULL, "%p");
    return 0;
}

TEST_CASE(vec_reserve_large) {
    vec_int v;
    vec_int_init(&v);
    ASSERT_EQ(vec_int_reserve(&v, 10000), 1, "%d");
    ASSERT_EQ(v.capacity, 10000, "%zu");
    vec_int_free(&v);
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
    vec_int_free(&v);
    return 0;
}

TEST_CASE(vec_reserve_grow) {
    vec_int v;
    vec_int_init(&v);
    ASSERT_EQ(vec_int_reserve(&v, 20), 1, "%d");
    ASSERT_GE(v.capacity, 20, "%zu");
    vec_int_free(&v);
    return 0;
}

TEST_CASE(vec_push_overflow) {
    vec_int v;
    vec_int_init(&v);
    v.capacity = (size_t)-1 / sizeof(int); // force next reserve to overflow
    v.size = v.capacity;
    int ret = vec_int_push(&v, 123);
    ASSERT_EQ(ret, 0, "%d");
    vec_int_free(&v);
    return 0;
}

TEST_MAIN
