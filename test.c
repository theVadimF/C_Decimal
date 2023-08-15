#include "c_decimal.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <check.h>

START_TEST(add_simple_test) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 18;
    test2.bits[LOW] = 69;
    c_decimal res = dec_add(test1, test2);
    int og[4];
    og[LOW]   = 0x57;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(add_test) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 0x80000000;
    test2.bits[MID] = 0x800;
    set_scale(&test1, 2);
    c_decimal res = dec_add(test1, test2);
    int og[4];
    og[LOW]   = 0x80000000;
    og[MID]   = 0x32000;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x20000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(add_neg_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 101;  // 101
    test2.bits[LOW] = 59;   // -5.9
    set_scale(&test2, 1);
    set_sign(&test2, 1);
    c_decimal res = dec_add(test1, test2);
    int og[4];
    og[LOW]   = 0x3B7;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x10000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(add_neg_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 1001;  // -100.1
    test2.bits[LOW] = 89;    // -8.9
    set_scale(&test1, 1);
    set_scale(&test2, 1);
    set_sign(&test1, 1);
    set_sign(&test2, 1);
    c_decimal res = dec_add(test1, test2);
    int og[4];
    og[LOW]   = 0x6d;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80000000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(add_neg_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 3;   // -3
    test2.bits[LOW] = 89;  // 8.9
    set_scale(&test2, 1);
    set_sign(&test1, 1);
    c_decimal res = dec_add(test1, test2);
    int og[4];
    og[LOW]   = 0x3B;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x10000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(add_neg_test4) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 250;   // -250
    test2.bits[LOW] = 893;   // 89.3
    set_scale(&test2, 1);
    set_sign(&test1, 1);
    c_decimal res = dec_add(test1, test2);
    int og[4];
    og[LOW]   = 0x647;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80010000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(add_neg_test5) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 32;
    test2.bits[LOW] = 893;  // -89.3
    set_scale(&test2, 1);
    set_sign(&test2, 1);
    c_decimal res = dec_add(test1, test2);
    int og[4];
    og[LOW]   = 0x23D;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80010000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(add_nan_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NAN;
    c_decimal res = dec_add(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(add_nan_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.value_type = DEC_NAN;
    c_decimal res = dec_add(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(add_nan_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NAN;
    test2.value_type = DEC_NAN;
    c_decimal res = dec_add(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(add_inf_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    c_decimal res = dec_add(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(add_inf_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    test2.value_type = DEC_INFINITY;
    c_decimal res = dec_add(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(add_inf_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    test2.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_add(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(add__neg_inf_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_add(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(add__neg_inf_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    test2.value_type = DEC_INFINITY;
    c_decimal res = dec_add(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(add__neg_inf_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    test2.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_add(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(add_overflow_test) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    // test1 all bits = 1
    test1.bits[LOW]  = -1;
    test1.bits[MID]  = -1;
    test1.bits[HIGH] = -1;
    test2.bits[LOW] = 550;
    c_decimal res = dec_add(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(add_overflow_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    // test1 all bits = 1
    test1.bits[LOW]  = -1;
    test1.bits[MID]  = -1;
    test1.bits[HIGH] = -1;
    set_scale(&test1, 28);
    test2.bits[HIGH] = 550;
    c_decimal res = dec_add(test1, test2);
    int og[4];
    // Verified via Wolfram Alpha
    og[LOW]   = 0x78e480;
    og[MID]   = 0x0;
    og[HIGH]  = 0x20c85580;
    og[SCALE] = 0x60000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(add_zero_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.bits[LOW] = 893;  // -89.3
    set_scale(&test2, 1);
    set_sign(&test2, 1);
    c_decimal res = dec_add(test1, test2);
    int og[4];
    og[LOW]   = 893;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80010000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(add_zero_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 893;  // -89.3
    set_scale(&test1, 1);
    set_sign(&test1, 1);
    c_decimal res = dec_add(test1, test2);
    int og[4];
    og[LOW]   = 893;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80010000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

Suite *add_suite(void) {
    Suite *s;
    TCase *tc;
    s = suite_create("add");
    tc = tcase_create("add");
    tcase_add_test(tc, add_simple_test);
    tcase_add_test(tc, add_test);
    tcase_add_test(tc, add_neg_test1);
    tcase_add_test(tc, add_neg_test2);
    tcase_add_test(tc, add_neg_test3);
    tcase_add_test(tc, add_neg_test4);
    tcase_add_test(tc, add_neg_test5);
    tcase_add_test(tc, add_nan_test1);
    tcase_add_test(tc, add_nan_test2);
    tcase_add_test(tc, add_nan_test3);
    tcase_add_test(tc, add_inf_test1);
    tcase_add_test(tc, add_inf_test2);
    tcase_add_test(tc, add_inf_test3);
    tcase_add_test(tc, add__neg_inf_test1);
    tcase_add_test(tc, add__neg_inf_test2);
    tcase_add_test(tc, add__neg_inf_test3);
    tcase_add_test(tc, add_overflow_test);
    tcase_add_test(tc, add_overflow_test2);
    tcase_add_test(tc, add_zero_test1);
    tcase_add_test(tc, add_zero_test2);
    suite_add_tcase(s, tc);
    return s;
}

START_TEST(negate_test1) {
    c_decimal test1 = init();
    test1.bits[MID]  = -1;
    set_scale(&test1, 28);
    set_sign(&test1, 1);
    c_decimal res = dec_negate(test1);
    int og[4];
    og[LOW]   = 0x0;
    og[MID]   = 0xffffffff;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x1c0000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(negate_test2) {
    c_decimal test1 = init();
    test1.bits[HIGH]  = -1;
    set_scale(&test1, 20);
    c_decimal res = dec_negate(test1);
    int og[4];
    og[LOW]   = 0x0;
    og[MID]   = 0x0;
    og[HIGH]  = 0xffffffff;
    og[SCALE] = 0x80140000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(negate_specials_test1) {
    c_decimal test1 = init();
    test1.value_type = DEC_NAN;
    c_decimal res = dec_negate(test1);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(negate_specials_test2) {
    c_decimal test1 = init();
    test1.value_type = DEC_INFINITY;
    c_decimal res = dec_negate(test1);
    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(negate_specials_test3) {
    c_decimal test1 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_negate(test1);
    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

Suite *negate_suite(void) {
    Suite *s;
    TCase *tc;
    s = suite_create("negate");
    tc = tcase_create("negate");
    tcase_add_test(tc, negate_test1);
    tcase_add_test(tc, negate_test2);
    tcase_add_test(tc, negate_specials_test1);
    tcase_add_test(tc, negate_specials_test2);
    tcase_add_test(tc, negate_specials_test3);
    suite_add_tcase(s, tc);
    return s;
}

START_TEST(sub_simple_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 1234;
    test2.bits[LOW] = 538;
    c_decimal res = dec_sub(test1, test2);
    int og[4];
    og[LOW]   = 0x2B8;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(sub_simple_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 27150;  // 27.150
    test2.bits[LOW] = 1253;   // 12.53
    set_scale(&test1, 3);
    set_scale(&test2, 2);
    c_decimal res = dec_sub(test1, test2);
    int og[4];
    og[LOW]   = 0x5B6;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x20000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(sub_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = 0xf12ba;
    test1.bits[MID]  = 0x00f12ba;
    test1.bits[HIGH] = 0x42ba;
    test2.bits[LOW]  = 0x0205ab;
    test2.bits[MID]  = 0x00a12aa;
    test2.bits[HIGH] = 0x42ba;
    set_scale(&test1, 2);
    set_scale(&test2, 5);
    c_decimal res = dec_sub(test1, test2);
    int og[4];
    og[LOW]   = 0x3adf20e5;
    og[MID]   = 0x3ad713e6;
    og[HIGH]  = 0x10463d6;
    og[SCALE] = 0x50000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(sub_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = 0xf12ba;
    test1.bits[MID]  = 0x00f12ba;
    test1.bits[HIGH] = 0x42ba;
    test2.bits[LOW]  = 0x0205ab;
    test2.bits[MID]  = 0xf00a12aa;
    test2.bits[HIGH] = 0xf42ba;
    set_scale(&test1, 2);
    set_scale(&test2, 2);
    c_decimal res = dec_sub(test1, test2);
    int og[4];
    og[LOW]   = 0xfff2f2f1;
    og[MID]   = 0xeffaffef;
    og[HIGH]  = 0xf0000;
    og[SCALE] = 0x80020000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(sub_sign_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 20131;  // 20.131
    test2.bits[LOW] = 1553;   // 15.53
    set_scale(&test1, 3);
    set_scale(&test2, 2);
    set_sign(&test2, 1);
    c_decimal res = dec_sub(test1, test2);
    int og[4];
    og[LOW]   = 0x8B4D;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x30000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(sub_sign_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 20131;  // 20.131
    test2.bits[LOW] = 1553;   // 15.53
    set_scale(&test1, 3);
    set_scale(&test2, 2);
    set_sign(&test1, 1);
    c_decimal res = dec_sub(test1, test2);
    int og[4];
    og[LOW]   = 0x8B4D;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80030000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(sub_sign_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 20131;  // 20.131
    test2.bits[LOW] = 1553;   // 15.53
    set_scale(&test1, 3);
    set_scale(&test2, 2);
    set_sign(&test1, 1);
    set_sign(&test2, 1);
    c_decimal res = dec_sub(test1, test2);
    int og[4];
    og[LOW]   = 0x11F9;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80030000;

    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(sub_inf_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    c_decimal res = dec_sub(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(sub_inf_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.value_type = DEC_INFINITY;
    c_decimal res = dec_sub(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(sub_inf_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    test2.value_type = DEC_INFINITY;
    c_decimal res = dec_sub(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(sub_inf_test4) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_sub(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(sub_inf_test5) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_sub(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(sub_inf_test6) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    test2.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_sub(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(sub_inf_test7) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    test2.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_sub(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(sub_inf_test8) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    test2.value_type = DEC_INFINITY;
    c_decimal res = dec_sub(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(sub_nan_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NAN;
    c_decimal res = dec_sub(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(sub_nan_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.value_type = DEC_NAN;
    c_decimal res = dec_sub(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(sub_nan_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NAN;
    test2.value_type = DEC_NAN;
    c_decimal res = dec_sub(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(sub_zero) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = 0xf12ba;
    test1.bits[MID]  = 0x00f12ba;
    test1.bits[HIGH] = 0x42ba;
    set_scale(&test1, 2);
    c_decimal res = dec_sub(test1, test2);
    int og[4];
    og[LOW]  = 0xf12ba;
    og[MID]  = 0x00f12ba;
    og[HIGH] = 0x42ba;
    og[SCALE] = 0x20000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(sub_neg_zero) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.bits[LOW]  = 0xf12ba;
    test2.bits[MID]  = 0x00f12ba;
    test2.bits[HIGH] = 0x42ba;
    set_scale(&test2, 2);
    c_decimal res = dec_sub(test1, test2);
    int og[4];
    og[LOW]  = 0xf12ba;
    og[MID]  = 0x00f12ba;
    og[HIGH] = 0x42ba;
    og[SCALE] = 0x80020000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(sub_same) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 365148;
    b.bits[LOW] = 365148;
    set_scale(&a, 2);
    set_scale(&b, 2);
    set_sign(&a, 1);
    set_sign(&b, 1);
    c_decimal res = dec_sub(a, b);
    int og[4];
    og[LOW]  = 0;
    og[MID]  = 0;
    og[HIGH] = 0;
    og[SCALE] = 0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

Suite *sub_suite(void) {
    Suite *s;
    TCase *tc;
    s = suite_create("sub");
    tc = tcase_create("sub");
    tcase_add_test(tc, sub_simple_test1);
    tcase_add_test(tc, sub_simple_test2);
    tcase_add_test(tc, sub_test1);
    tcase_add_test(tc, sub_test2);
    tcase_add_test(tc, sub_sign_test1);
    tcase_add_test(tc, sub_sign_test2);
    tcase_add_test(tc, sub_sign_test3);
    tcase_add_test(tc, sub_inf_test1);
    tcase_add_test(tc, sub_inf_test2);
    tcase_add_test(tc, sub_inf_test3);
    tcase_add_test(tc, sub_inf_test4);
    tcase_add_test(tc, sub_inf_test5);
    tcase_add_test(tc, sub_inf_test6);
    tcase_add_test(tc, sub_inf_test7);
    tcase_add_test(tc, sub_inf_test8);
    tcase_add_test(tc, sub_nan_test1);
    tcase_add_test(tc, sub_nan_test2);
    tcase_add_test(tc, sub_nan_test3);
    tcase_add_test(tc, sub_zero);
    tcase_add_test(tc, sub_neg_zero);
    tcase_add_test(tc, sub_same);

    suite_add_tcase(s, tc);
    return s;
}

START_TEST(mul_max) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    // This maxes out both decimals because we use signed int
    test1.bits[LOW]  = -1;
    test1.bits[MID]  = -1;
    test1.bits[HIGH] = -1;
    test2.bits[LOW]  = -1;
    test2.bits[MID]  = -1;
    test2.bits[HIGH] = -1;
    set_sign(&test1, 1);
    set_scale(&test1, 20);
    set_scale(&test2, 10);
    c_decimal res = dec_mul(test1, test2);
    int og[4];
    // Verified via WolframAlpha
    og[LOW]   = 0x3424b06f;
    og[MID]   = 0xebc29f86;
    og[HIGH]  = 0x14484bfe;
    og[SCALE] = 0x80000000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mul_overflow) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = -1;
    test1.bits[MID]  = -1;
    test1.bits[HIGH] = -1;
    test2.bits[LOW]  = -1;
    test2.bits[MID]  = -1;
    test2.bits[HIGH] = -1;
    set_scale(&test1, 15);
    c_decimal res = dec_mul(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(mul_negative_overflow) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = -1;
    test1.bits[MID]  = -1;
    test1.bits[HIGH] = -1;
    test2.bits[LOW]  = -1;
    test2.bits[MID]  = -1;
    test2.bits[HIGH] = -1;
    set_scale(&test1, 12);
    set_sign(&test1, 1);
    c_decimal res = dec_mul(test1, test2);

    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(mul_simple_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = 420;
    test2.bits[LOW]  = 69;
    c_decimal res = dec_mul(test1, test2);
    int og[4];
    og[LOW]   = 0x7134;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mul_simple_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = 420;
    test2.bits[LOW]  = 69;
    set_sign(&test1, 1);
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    int og[4];
    og[LOW]   = 0x7134;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mul_simple_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = 420;
    test2.bits[LOW]  = 69;
    set_sign(&test1, 1);
    c_decimal res = dec_mul(test1, test2);
    int og[4];
    og[LOW]   = 0x7134;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80000000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mul_simple_test4) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = 420;
    test2.bits[LOW]  = 69;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    int og[4];
    og[LOW]   = 0x7134;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80000000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mul_zero_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = 420;
    test2.bits[LOW]  = 0;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    int og[4];
    og[LOW]   = 0x0;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mul_zero_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = 0;
    test2.bits[LOW]  = 69;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    int og[4];
    og[LOW]   = 0x0;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mul_zero_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = 0;
    test2.bits[LOW]  = 69;
    c_decimal res = dec_mul(test1, test2);
    int og[4];
    og[LOW]   = 0x0;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mul_zero_test4) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW]  = 420;
    test2.bits[LOW]  = 0;
    c_decimal res = dec_mul(test1, test2);
    int og[4];
    og[LOW]   = 0x0;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mul_nan_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NAN;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(mul_nan_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.value_type = DEC_NAN;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(mul_nan_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NAN;
    test2.value_type = DEC_NAN;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(mul_inf_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(mul_inf_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    test2.bits[LOW] = 98;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(mul_inf_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    test2.bits[LOW] = 123;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(mul_inf_test4) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    test2.value_type = DEC_NEGATIVE_INFINITY;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(mul_inf_test5) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    test2.value_type = DEC_INFINITY;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(mul_inf_test6) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_INFINITY;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(mul_inf_test7) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.value_type = DEC_INFINITY;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(mul_inf_test8) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.value_type = DEC_INFINITY;
    set_sign(&test1, 1);
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(mul_inf_test9) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.value_type = DEC_INFINITY;
    test1.bits[MID] = 123;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(mul_inf_test10) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.value_type = DEC_INFINITY;
    test1.bits[MID] = 23;
    set_sign(&test1, 1);
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(mul_neginf_test1) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(mul_neginf_test2) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(mul_neginf_test3) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    test2.bits[MID] = 1245;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(mul_neginf_test4) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    test2.bits[HIGH] = 412;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(mul_neginf_test5) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    test2.value_type = DEC_INFINITY;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(mul_neginf_test6) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.value_type = DEC_NEGATIVE_INFINITY;
    test2.value_type = DEC_NEGATIVE_INFINITY;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

START_TEST(mul_neginf_test7) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test2.value_type = DEC_NEGATIVE_INFINITY;
    set_sign(&test2, 1);
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(mul_neginf_test8) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    set_sign(&test1, 1);
    test2.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

START_TEST(mul_neginf_test9) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 4668;
    set_scale(&test1, 3);
    test2.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_NEGATIVE_INFINITY);
}

START_TEST(mul_neginf_test10) {
    c_decimal test1 = init();
    c_decimal test2 = init();
    test1.bits[LOW] = 4668;
    set_sign(&test1, 1);
    test2.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal res = dec_mul(test1, test2);
    ck_assert_int_eq(res.value_type, DEC_INFINITY);
}

Suite *mul_suite(void) {
    Suite *s;
    TCase *tc;
    s = suite_create("mul");
    tc = tcase_create("mul");
    tcase_add_test(tc, mul_max);
    tcase_add_test(tc, mul_overflow);
    tcase_add_test(tc, mul_negative_overflow);
    tcase_add_test(tc, mul_simple_test1);
    tcase_add_test(tc, mul_simple_test2);
    tcase_add_test(tc, mul_simple_test3);
    tcase_add_test(tc, mul_simple_test4);
    tcase_add_test(tc, mul_zero_test1);
    tcase_add_test(tc, mul_zero_test2);
    tcase_add_test(tc, mul_zero_test3);
    tcase_add_test(tc, mul_zero_test4);
    tcase_add_test(tc, mul_nan_test1);
    tcase_add_test(tc, mul_nan_test2);
    tcase_add_test(tc, mul_nan_test3);
    tcase_add_test(tc, mul_inf_test1);
    tcase_add_test(tc, mul_inf_test2);
    tcase_add_test(tc, mul_inf_test3);
    tcase_add_test(tc, mul_inf_test4);
    tcase_add_test(tc, mul_inf_test5);
    tcase_add_test(tc, mul_inf_test6);
    tcase_add_test(tc, mul_inf_test7);
    tcase_add_test(tc, mul_inf_test8);
    tcase_add_test(tc, mul_inf_test9);
    tcase_add_test(tc, mul_inf_test10);
    tcase_add_test(tc, mul_neginf_test1);
    tcase_add_test(tc, mul_neginf_test2);
    tcase_add_test(tc, mul_neginf_test3);
    tcase_add_test(tc, mul_neginf_test4);
    tcase_add_test(tc, mul_neginf_test5);
    tcase_add_test(tc, mul_neginf_test6);
    tcase_add_test(tc, mul_neginf_test7);
    tcase_add_test(tc, mul_neginf_test8);
    tcase_add_test(tc, mul_neginf_test9);
    tcase_add_test(tc, mul_neginf_test10);

    suite_add_tcase(s, tc);
    return s;
}

START_TEST(flt2dec_zero) {
    c_decimal dst = init();
    float src = 0;
    int code = from_float_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0x0;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_min) {
    c_decimal dst = init();
    float src = 1e-28;
    int code = from_float_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0x1;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x1c0000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_too_small) {
    c_decimal dst = init();
    float src = 1e-30;
    int code = from_float_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0x0;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, CALCULATION_ERROR);
}

START_TEST(flt2dec_too_small_neg) {
    c_decimal dst = init();
    float src = -1e-30;
    int code = from_float_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0x0;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, CALCULATION_ERROR);
}

START_TEST(flt2dec_low) {
    c_decimal dst = init();
    float src = 0.0000000000125;
    int code = from_float_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0x7d;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0xd0000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_scale_min) {
    c_decimal dst = init();
    float src = 0.00000000000000000000100025;
    int code = from_float_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 100024;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 1703936;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_scale_cutoff) {
    c_decimal dst = init();
    float src = 0.00000000000000000000000000125;
    int code = from_float_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0xc;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x1c0000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_test1) {
    c_decimal dst = init();
    float src = 1234.567;
    int code = from_float_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 123456;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 131072;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_neg_test1) {
    c_decimal dst = init();
    float src = -1234.567;
    int code = from_float_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 123456;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = -2147352576;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_too_big) {
    c_decimal dst = init();
    float src = 1009228162514264337593543950336.0;
    int code = from_float_to_decimal(src, &dst);
    ck_assert_int_eq(dst.value_type, DEC_INFINITY);
    ck_assert_int_eq(code, CALCULATION_ERROR);
}

START_TEST(flt2dec_too_big_neg) {
    c_decimal dst = init();
    float src = -1009228162514264337593543950336.0;
    int code = from_float_to_decimal(src, &dst);
    ck_assert_int_eq(dst.value_type, DEC_NEGATIVE_INFINITY);
    ck_assert_int_eq(code, CALCULATION_ERROR);
}

START_TEST(flt2dec_max) {
    c_decimal dst = init();
    float src = FLOAT_MAX;
    int code = from_float_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0xffffffff;
    og[MID]   = 0xffffffff;
    og[HIGH]  = 0xffffffff;
    og[SCALE] = 0x0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_max_neg) {
    c_decimal dst = init();
    float src = -FLOAT_MAX;
    int code = from_float_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0xffffffff;
    og[MID]   = 0xffffffff;
    og[HIGH]  = 0xffffffff;
    og[SCALE] = 0x80000000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_nan) {
    c_decimal dst = init();
    float src = NAN;
    int code = from_float_to_decimal(src, &dst);
    ck_assert_int_eq(dst.value_type, DEC_NAN);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_inf) {
    c_decimal dst = init();
    float src = INFINITY;
    int code = from_float_to_decimal(src, &dst);
    ck_assert_int_eq(dst.value_type, DEC_INFINITY);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_neg_inf) {
    c_decimal dst = init();
    float src = -INFINITY;
    int code = from_float_to_decimal(src, &dst);
    ck_assert_int_eq(dst.value_type, DEC_NEGATIVE_INFINITY);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(flt2dec_null) {
    float src = 1244;
    int code = from_float_to_decimal(src, NULL);
    ck_assert_int_eq(code, CALCULATION_ERROR);
}

Suite *float_to_decimal_suite(void) {
    Suite *s;
    TCase *tc;
    s = suite_create("float_to_decimal");
    tc = tcase_create("float_to_decimal");

    tcase_add_test(tc, flt2dec_min);
    tcase_add_test(tc, flt2dec_zero);
    tcase_add_test(tc, flt2dec_too_small);
    tcase_add_test(tc, flt2dec_low);
    tcase_add_test(tc, flt2dec_test1);
    tcase_add_test(tc, flt2dec_scale_min);
    tcase_add_test(tc, flt2dec_scale_cutoff);
    tcase_add_test(tc, flt2dec_neg_test1);
    tcase_add_test(tc, flt2dec_too_small_neg);
    tcase_add_test(tc, flt2dec_too_big);
    tcase_add_test(tc, flt2dec_too_big_neg);
    tcase_add_test(tc, flt2dec_max);
    tcase_add_test(tc, flt2dec_max_neg);
    tcase_add_test(tc, flt2dec_nan);
    tcase_add_test(tc, flt2dec_inf);
    tcase_add_test(tc, flt2dec_neg_inf);
    tcase_add_test(tc, flt2dec_null);

    suite_add_tcase(s, tc);
    return s;
}

START_TEST(int2dec_zero) {
    c_decimal dst = init();
    int src = 0;
    int code = from_int_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0x0;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(int2dec_test) {
    c_decimal dst = init();
    int src = 283745;
    int code = from_int_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0x45461;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(int2dec_neg_test) {
    c_decimal dst = init();
    int src = -970834;
    int code = from_int_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0xed052;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80000000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(int2dec_null) {
    int src = 1;
    int code = from_int_to_decimal(src, NULL);
    ck_assert_int_eq(code, CALCULATION_ERROR);
}

START_TEST(int2dec_max) {
    c_decimal dst = init();
    int src = 2147483647;
    int code = from_int_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0x7fffffff;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

START_TEST(int2dec_neg_max) {
    c_decimal dst = init();
    int src = -2147483647;
    int code = from_int_to_decimal(src, &dst);
    int og[4];
    og[LOW]   = 0x7fffffff;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80000000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(dst.bits[i], og[i]);
    }
    ck_assert_int_eq(dst.value_type, DEC_NORMAL_VALUE);
    ck_assert_int_eq(code, SUCCESS);
}

Suite *int_to_decimal_suite(void) {
    Suite *s = suite_create("int_to_decimal");
    TCase *tc = tcase_create("int_to_decimal");
    tcase_add_test(tc, int2dec_zero);
    tcase_add_test(tc, int2dec_test);
    tcase_add_test(tc, int2dec_neg_test);
    tcase_add_test(tc, int2dec_null);
    tcase_add_test(tc, int2dec_max);
    tcase_add_test(tc, int2dec_neg_max);

    suite_add_tcase(s, tc);
    return s;
}

// start mod test

START_TEST(mod_test) {
    c_decimal a = init();
    c_decimal b = init();
    b.value_type = DEC_NAN;

    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(res, b);
    int excepted = FALSE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_test_2) {
    c_decimal a = init();
    c_decimal b = init();

    c_decimal ex = init();
    ex.value_type = DEC_NAN;

    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = FALSE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_test_3) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_INFINITY;

    c_decimal ex = init();
    ex.value_type = DEC_NAN;

    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = FALSE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_test_4) {
    c_decimal a = init();
    c_decimal b = init();
    set_bit(&a, 1);

    c_decimal ex = init();
    ex.value_type = DEC_NAN;

    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = FALSE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_test_5) {
    c_decimal a = init();
    c_decimal b = init();

    b.value_type = DEC_INFINITY;

    c_decimal ex = init();


    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_test_6) {
    c_decimal a = init();
    c_decimal b = init();

    set_bit(&a, 5);

    c_decimal ex = init();
    ex.value_type = DEC_NAN;

    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = FALSE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_test_7) {
    c_decimal a = init();
    c_decimal b = init();

    set_bit(&a, 5);
    set_bit(&b, 1);

    c_decimal ex = init();


    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_test_8) {
    c_decimal a = init();
    c_decimal b = init();

    set_bit(&a, 5);
    set_bit(&a, 0);
    set_bit(&b, 1);

    c_decimal ex = init();
    set_bit(&ex, 0);

    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_test_10) {
    c_decimal a = init();
    c_decimal b = init();

    set_bit(&a, 5);
    set_bit(&a, 0);
    set_bit(&b, 1);
    set_sign(&a, 1);
    set_sign(&b, 1);

    c_decimal ex = init();
    set_sign(&ex, 1);
    set_bit(&ex, 0);

    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_test_9) {
    c_decimal a = init();
    c_decimal b = init();

    set_bit(&a, 5);
    set_bit(&a, 0);
    set_bit(&b, 1);
    set_sign(&a, 1);

    c_decimal ex = init();
    set_sign(&ex, 1);
    set_bit(&ex, 0);

    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_test_11) {
    c_decimal a = init();
    c_decimal b = init();

    set_bit(&a, 5);
    set_bit(&a, 0);
    set_bit(&b, 1);
    set_sign(&b, 1);

    c_decimal ex = init();
    set_bit(&ex, 0);

    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_test_12) {
    c_decimal a = init();
    c_decimal b = init();

    set_bit(&a, 5);
    set_bit(&b, 1);
    set_bit(&a, 0);
    set_sign(&a, 1);

    c_decimal ex = init();
    set_bit(&ex, 0);
    set_sign(&ex, 1);

    c_decimal res = dec_mod(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(mod_complex_test) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = -1;  // This sets all bits to 1
    b.bits[LOW] = 6487;
    set_scale(&b, 28);
    c_decimal res = dec_mod(a, b);
    int og[4];
    og[LOW]   = 0x1065;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x1c0000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mod_neg_test1) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = -1;  // This sets all bits to 1
    b.bits[LOW] = 6487;
    set_scale(&b, 28);
    set_sign(&b, 1);
    c_decimal res = dec_mod(a, b);
    int og[4];
    og[LOW]   = 0x1065;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x1c0000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mod_neg_test2) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = -1;  // This sets all bits to 1
    b.bits[LOW] = 6487;
    set_scale(&b, 28);
    set_sign(&a, 1);
    c_decimal res = dec_mod(a, b);
    int og[4];
    og[LOW]   = 0x1065;
    og[MID]   = 0x0;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x801c0000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mod_inf_special) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 1;  // This sets all bits to 1
    b.value_type = DEC_INFINITY;
    c_decimal res = dec_mod(a, b);
    int og[4];
    og[LOW]   = 1;
    og[MID]   = 0;
    og[HIGH]  = 0;
    og[SCALE] = 0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

START_TEST(mod_div_neg_zero) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 465;
    set_sign(&b, 1);
    c_decimal res = dec_mod(a, b);
    ck_assert_int_eq(res.value_type, DEC_NAN);
}

Suite *mod_suite(void) {
    Suite *s = suite_create("mod");
    TCase *tc = tcase_create("mod");
    tcase_add_test(tc, mod_test);
    tcase_add_test(tc, mod_test_2);
    tcase_add_test(tc, mod_test_3);
    tcase_add_test(tc, mod_test_4);
    tcase_add_test(tc, mod_test_5);
    tcase_add_test(tc, mod_test_6);
    tcase_add_test(tc, mod_test_7);
    tcase_add_test(tc, mod_test_8);
    tcase_add_test(tc, mod_test_9);
    tcase_add_test(tc, mod_test_10);
    tcase_add_test(tc, mod_test_11);
    tcase_add_test(tc, mod_test_12);
    tcase_add_test(tc, mod_complex_test);
    tcase_add_test(tc, mod_neg_test1);
    tcase_add_test(tc, mod_neg_test2);
    tcase_add_test(tc, mod_inf_special);
    tcase_add_test(tc, mod_div_neg_zero);

    suite_add_tcase(s, tc);
    return s;
}

// end mod test

// start div test

START_TEST(div_test) {
    c_decimal a = init();
    c_decimal b = init();


    c_decimal ex = init();
    ex.value_type = DEC_NAN;

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = FALSE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_2) {
    c_decimal a = init();
    c_decimal b = init();
    set_bit(&a, 0);

    c_decimal ex = init();
    ex.value_type = DEC_INFINITY;

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_3) {
    c_decimal a = init();
    c_decimal b = init();
    set_bit(&a, 0);
    set_sign(&a, 1);

    c_decimal ex = init();
    ex.value_type = DEC_NEGATIVE_INFINITY;

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_4) {
    c_decimal a = init();
    c_decimal b = init();
    set_bit(&a, 5);
    set_bit(&b, 1);

    c_decimal ex = init();
    set_bit(&ex, 4);

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_5) {
    c_decimal a = init();
    c_decimal b = init();
    set_bit(&a, 5);
    set_bit(&b, 1);
    set_sign(&a, 1);
    set_sign(&b, 1);

    c_decimal ex = init();
    set_bit(&ex, 4);

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_6) {
    c_decimal a = init();
    c_decimal b = init();
    set_bit(&a, 5);
    set_bit(&b, 1);
    set_sign(&a, 1);

    c_decimal ex = init();
    set_bit(&ex, 4);
    set_sign(&ex, 1);

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_7) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_NAN;
    b.value_type = DEC_NAN;

    c_decimal ex = init();
    ex.value_type = DEC_NAN;

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = FALSE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_8) {
    c_decimal a = init();
    c_decimal b = init();
    b.value_type = DEC_INFINITY;

    c_decimal ex = init();

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_9) {
    c_decimal a = init();
    c_decimal b = init();
    set_sign(&a, 1);
    b.value_type = DEC_INFINITY;

    c_decimal ex = init();
    set_sign(&ex, 1);

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_10) {
    c_decimal a = init();
    c_decimal b = init();
    set_sign(&a, 1);
    b.value_type = DEC_NEGATIVE_INFINITY;

    c_decimal ex = init();
    set_sign(&ex, 0);

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_11) {
    c_decimal a = init();
    c_decimal b = init();

    b.value_type = DEC_NEGATIVE_INFINITY;

    c_decimal ex = init();
    set_sign(&ex, 1);

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_12) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_INFINITY;
    set_sign(&a, 0);

    c_decimal ex = init();
    ex.value_type = DEC_INFINITY;

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_13) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_INFINITY;
    set_sign(&b, 1);

    c_decimal ex = init();
    ex.value_type = DEC_NEGATIVE_INFINITY;

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_14) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_NEGATIVE_INFINITY;
    set_sign(&b, 0);

    c_decimal ex = init();
    ex.value_type = DEC_NEGATIVE_INFINITY;

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_15) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_NEGATIVE_INFINITY;
    set_sign(&b, 1);

    c_decimal ex = init();
    ex.value_type = DEC_INFINITY;

    c_decimal res = dec_div(a, b);
    int actual_res = dec_is_equal(ex, res);
    int excepted = TRUE;
    ck_assert_int_eq(actual_res, excepted);
}

START_TEST(div_test_complex1) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = -1;  // 4294967295
    b.bits[LOW] = 65148;
    set_sign(&b, 1);
    c_decimal res = dec_div(a, b);
    int og[4];
    og[LOW]   = 0x3d0ee99a;
    og[MID]   = 0xd5bf7b68;
    og[HIGH]  = 0xd504f754;
    og[SCALE] = 0x80180000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(res.bits[i], og[i]);
    }
    ck_assert_int_eq(res.value_type, DEC_NORMAL_VALUE);
}

Suite *div_suite(void) {
    Suite *s = suite_create("div");
    TCase *tc = tcase_create("div");
    tcase_add_test(tc, div_test);
    tcase_add_test(tc, div_test_2);
    tcase_add_test(tc, div_test_3);
    tcase_add_test(tc, div_test_4);
    tcase_add_test(tc, div_test_5);
    tcase_add_test(tc, div_test_6);
    tcase_add_test(tc, div_test_7);
    tcase_add_test(tc, div_test_8);
    tcase_add_test(tc, div_test_9);
    tcase_add_test(tc, div_test_10);
    tcase_add_test(tc, div_test_11);
    tcase_add_test(tc, div_test_12);
    tcase_add_test(tc, div_test_13);
    tcase_add_test(tc, div_test_14);
    tcase_add_test(tc, div_test_15);
    tcase_add_test(tc, div_test_complex1);

    suite_add_tcase(s, tc);
    return s;
}


// end div test

// equal operators section

// start equals test

START_TEST(dec_is_equal_zero) {
    c_decimal a = init();
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_equal_scale_equal) {
    c_decimal a = init();
    c_decimal b = init();
    set_bit(&a, 8);
    set_bit(&a, 7);
    set_bit(&a, 6);
    set_bit(&a, 5);
    set_bit(&a, 4);
    set_bit(&a, 2);
    set_bit(&b, 2);
    set_bit(&b, 0);
    set_scale(&a, 2);

    int excepted = TRUE;
    int recieved = dec_is_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_equal_one) {
    c_decimal a = init();
    set_bit(&a, 0);
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_equal_mone) {
    c_decimal a = init();
    set_bit(&a, 0);
    c_decimal b = init();
    set_sign(&a, 1);

    int excepted = FALSE;
    int recieved = dec_is_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_equal_neg_inf) {
    c_decimal a = init();
    c_decimal b = init();
    b.value_type = DEC_NEGATIVE_INFINITY;
    int excepted = FALSE;
    int recieved = dec_is_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}


START_TEST(dec_is_equal_inf) {
    c_decimal a = init();
    c_decimal b = init();
    b.value_type = DEC_NORMAL_VALUE;
    b.value_type = DEC_INFINITY;
    int excepted = FALSE;
    int recieved = dec_is_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}


START_TEST(dec_is_equal_nan) {
    c_decimal a = init();
    set_bit(&a, 0);
    c_decimal b = init();
    b.value_type = DEC_NAN;
    int excepted = FALSE;
    int recieved = dec_is_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_equal_scale) {
    c_decimal a = init();
    set_scale(&a, 10);
    c_decimal b = init();
    int excepted = TRUE;
    int recieved = dec_is_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_equal_scale_2) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 10);
    set_scale(&b, 10);
    int excepted = TRUE;
    int recieved = dec_is_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_equal_scale_3) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 11);
    set_scale(&b, 11);
    int excepted = TRUE;
    int recieved = dec_is_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}


START_TEST(dec_is_equal_neg_zero) {
    c_decimal a = init();
    c_decimal b = init();
    set_sign(&a, 1);
    int excepted = TRUE;
    int recieved = dec_is_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

Suite* is_equal_suite(void) {
    Suite *s;
    TCase *tc_is_equal;
    s = suite_create("is_equal");
    tc_is_equal = tcase_create("is_equal");
    tcase_add_test(tc_is_equal, dec_is_equal_zero);
    tcase_add_test(tc_is_equal, dec_is_equal_one);
    tcase_add_test(tc_is_equal, dec_is_equal_mone);
    tcase_add_test(tc_is_equal, dec_is_equal_neg_inf);
    tcase_add_test(tc_is_equal, dec_is_equal_inf);
    tcase_add_test(tc_is_equal, dec_is_equal_nan);
    tcase_add_test(tc_is_equal, dec_is_equal_scale);
    tcase_add_test(tc_is_equal, dec_is_equal_scale_2);
    tcase_add_test(tc_is_equal, dec_is_equal_scale_3);
    tcase_add_test(tc_is_equal, dec_is_equal_scale_equal);
    tcase_add_test(tc_is_equal, dec_is_equal_neg_zero);
    suite_add_tcase(s, tc_is_equal);
    return s;
}


// start is less test

START_TEST(dec_is_less_zero) {
    c_decimal a = init();
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_less(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_one) {
    c_decimal a = init();
    set_bit(&a, 0);
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_less(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_mone) {
    c_decimal a = init();
    set_bit(&a, 0);
    set_sign(&a, 1);
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_less(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_inf) {
    c_decimal a = init();
    a.value_type = DEC_INFINITY;
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_less(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_neginf) {
    c_decimal a = init();
    a.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_less(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_nan) {
    c_decimal a = init();
    a.value_type = DEC_NAN;
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_less(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_scale) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 10);
    set_scale(&b, 15);

    int excepted = FALSE;
    int recieved = dec_is_less(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_scale_2) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 15);
    set_scale(&b, 20);

    int excepted = FALSE;
    int recieved = dec_is_less(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_scale_3) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 15);
    set_scale(&b, 20);
    set_sign(&a, 1);
    set_sign(&b, 1);

    int excepted = FALSE;
    int recieved = dec_is_less(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(is_less_test1) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[HIGH] = 12532;
    b.bits[HIGH] = 125;
    int res = dec_is_less(b, a);
    ck_assert_int_eq(res, TRUE);
}

START_TEST(is_less_test2) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 69487;
    b.bits[LOW] = 69487812;
    set_scale(&a, 1);
    set_scale(&b, 3);
    int res = dec_is_less(a, b);
    ck_assert_int_eq(res, TRUE);
}

START_TEST(is_less_false_test1) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[HIGH] = 12532;
    b.bits[HIGH] = 125;
    int res = dec_is_less(a, b);
    ck_assert_int_eq(res, FALSE);
}

START_TEST(is_less_false_test2) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 69487;
    b.bits[LOW] = 69487812;
    set_scale(&a, 1);
    set_scale(&b, 5);
    int res = dec_is_less(a, b);
    ck_assert_int_eq(res, FALSE);
}

START_TEST(is_less_neg_test1) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 69487;
    b.bits[LOW] = 69487812;  // 694.87812
    set_scale(&a, 1);
    set_scale(&b, 5);
    set_sign(&a, 1);
    set_sign(&b, 1);
    int res = dec_is_less(a, b);
    ck_assert_int_eq(res, TRUE);
}

START_TEST(is_less_neg_test2) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 69487;
    b.bits[LOW] = 69487812;  // 694.87812
    set_scale(&a, 1);
    set_scale(&b, 5);
    set_sign(&b, 1);
    int res = dec_is_less(a, b);
    ck_assert_int_eq(res, FALSE);
}

START_TEST(is_less_neg_test3) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 86484;
    b.bits[LOW] = 12484;
    set_sign(&a, 1);
    set_sign(&b, 1);
    int res = dec_is_less(a, b);
    ck_assert_int_eq(res, TRUE);
}

START_TEST(is_less_neg_test4) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 1;
    b.bits[LOW] = 1;
    set_sign(&a, 1);
    set_sign(&b, 1);
    int res = dec_is_less(a, b);
    ck_assert_int_eq(res, FALSE);
}

START_TEST(is_less_equal_test) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 69487000;
    b.bits[LOW] = 69487;
    set_scale(&a, 3);
    int res = dec_is_less(a, b);
    ck_assert_int_eq(res, FALSE);
}

Suite* is_less_suite(void) {
    Suite *s;
    TCase *tc_is_less;
    s = suite_create("is_less");
    tc_is_less = tcase_create("is_less");
    tcase_add_test(tc_is_less, dec_is_less_zero);
    tcase_add_test(tc_is_less, dec_is_less_one);
    tcase_add_test(tc_is_less, dec_is_less_one);
    tcase_add_test(tc_is_less, dec_is_less_mone);
    tcase_add_test(tc_is_less, dec_is_less_inf);
    tcase_add_test(tc_is_less, dec_is_less_neginf);
    tcase_add_test(tc_is_less, dec_is_less_nan);
    tcase_add_test(tc_is_less, dec_is_less_scale);
    tcase_add_test(tc_is_less, dec_is_less_scale_2);
    tcase_add_test(tc_is_less, dec_is_less_scale_3);
    tcase_add_test(tc_is_less, is_less_test1);
    tcase_add_test(tc_is_less, is_less_test2);
    tcase_add_test(tc_is_less, is_less_false_test1);
    tcase_add_test(tc_is_less, is_less_false_test2);
    tcase_add_test(tc_is_less, is_less_neg_test1);
    tcase_add_test(tc_is_less, is_less_neg_test2);
    tcase_add_test(tc_is_less, is_less_neg_test3);
    tcase_add_test(tc_is_less, is_less_neg_test4);
    tcase_add_test(tc_is_less, is_less_equal_test);
    suite_add_tcase(s, tc_is_less);
    return s;
}
// end is less test

// start is greater test

START_TEST(dec_is_greater_zero) {
    c_decimal a = init();
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_greater(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_one) {
    c_decimal a = init();
    set_bit(&a, 0);
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_greater(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_mone) {
    c_decimal a = init();
    set_bit(&a, 0);
    set_sign(&a, 1);
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_greater(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_inf) {
    c_decimal a = init();
    a.value_type = DEC_INFINITY;
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_greater(a, b);
}

START_TEST(dec_is_greater_neginf) {
    c_decimal a = init();
    a.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_greater(a, b);
}

START_TEST(dec_is_greater_nan) {
    c_decimal a = init();
    set_bit(&a, 0);
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_greater(a, b);
}

START_TEST(dec_is_greater_scale) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 10);
    set_scale(&b, 15);

    int excepted = FALSE;
    int recieved = dec_is_greater(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_scale_2) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 15);
    set_scale(&b, 20);

    int excepted = FALSE;
    int recieved = dec_is_greater(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_scale_3) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 15);
    set_scale(&b, 20);
    set_sign(&a, 1);
    set_sign(&b, 1);

    int excepted = FALSE;
    int recieved = dec_is_greater(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(is_greater_test1) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[HIGH] = 365484;
    b.bits[HIGH] = 15484;

    int excepted = TRUE;
    int recieved = dec_is_greater(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(is_greater_test2) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 365484;
    b.bits[LOW] = 15484;
    set_scale(&a, 3);

    int excepted = TRUE;
    int recieved = dec_is_greater(b, a);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(is_greater_equal_test) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 154000;
    b.bits[LOW] = 154;
    set_scale(&a, 3);

    int excepted = FALSE;
    int recieved = dec_is_greater(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(is_greater_neg_test1) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 154000;
    b.bits[LOW] = 154;
    set_scale(&a, 3);
    set_sign(&b, 1);

    int excepted = TRUE;
    int recieved = dec_is_greater(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(is_greater_neg_test2) {
    c_decimal a = init();
    c_decimal b = init();
    a.bits[LOW] = 154000;
    b.bits[LOW] = 154;
    set_scale(&a, 3);
    set_sign(&a, 1);

    int excepted = FALSE;
    int recieved = dec_is_greater(a, b);
    ck_assert_int_eq(recieved, excepted);
}

Suite* is_greater_suite(void) {
    Suite *s;
    TCase *tc_is_greater;
    s = suite_create("is_greater");
    tc_is_greater = tcase_create("is_greater");
    tcase_add_test(tc_is_greater, dec_is_greater_zero);
    tcase_add_test(tc_is_greater, dec_is_greater_one);
    tcase_add_test(tc_is_greater, dec_is_greater_mone);
    tcase_add_test(tc_is_greater, dec_is_greater_inf);
    tcase_add_test(tc_is_greater, dec_is_greater_neginf);
    tcase_add_test(tc_is_greater, dec_is_greater_nan);
    tcase_add_test(tc_is_greater, dec_is_greater_scale);
    tcase_add_test(tc_is_greater, dec_is_greater_scale_2);
    tcase_add_test(tc_is_greater, dec_is_greater_scale_3);
    tcase_add_test(tc_is_greater, is_greater_test1);
    tcase_add_test(tc_is_greater, is_greater_test2);
    tcase_add_test(tc_is_greater, is_greater_equal_test);
    tcase_add_test(tc_is_greater, is_greater_neg_test1);
    tcase_add_test(tc_is_greater, is_greater_neg_test2);
    suite_add_tcase(s, tc_is_greater);
    return s;
}
// end is greater test

// start is not equal test

START_TEST(dec_is_not_equal_zero) {
    c_decimal a = init();
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_not_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_not_equal_one) {
    c_decimal a = init();
    set_bit(&a, 0);
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_not_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_not_equal_mone) {
    c_decimal a = init();
    set_bit(&a, 0);
    c_decimal b = init();
    set_sign(&a, 1);

    int excepted = TRUE;
    int recieved = dec_is_not_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_not_equal_neg_inf) {
    c_decimal a = init();
    c_decimal b = init();
    b.value_type = DEC_NEGATIVE_INFINITY;
    int excepted = TRUE;
    int recieved = dec_is_not_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}


START_TEST(dec_is_not_equal_inf) {
    c_decimal a = init();
    c_decimal b = init();
    b.value_type = DEC_NORMAL_VALUE;
    b.value_type = DEC_INFINITY;
    int excepted = TRUE;
    int recieved = dec_is_not_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}


START_TEST(dec_is_not_equal_nan) {
    c_decimal a = init();
    set_bit(&a, 0);
    c_decimal b = init();
    b.value_type = DEC_NAN;
    int excepted = TRUE;
    int recieved = dec_is_not_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_not_equal_scale) {
    c_decimal a = init();
    set_scale(&a, 10);
    c_decimal b = init();
    int excepted = FALSE;
    int recieved = dec_is_not_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_not_equal_scale_2) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 10);
    set_scale(&b, 10);
    int excepted = FALSE;
    int recieved = dec_is_not_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

Suite* is_not_equal_suite(void) {
    Suite *s;
    TCase *tc_is_not_equal;
    s = suite_create("is_not_equal");
    tc_is_not_equal = tcase_create("is_not_equal");
    tcase_add_test(tc_is_not_equal, dec_is_not_equal_zero);
    tcase_add_test(tc_is_not_equal, dec_is_not_equal_one);
    tcase_add_test(tc_is_not_equal, dec_is_not_equal_mone);
    tcase_add_test(tc_is_not_equal, dec_is_not_equal_neg_inf);
    tcase_add_test(tc_is_not_equal, dec_is_not_equal_inf);
    tcase_add_test(tc_is_not_equal, dec_is_not_equal_nan);
    tcase_add_test(tc_is_not_equal, dec_is_not_equal_scale);
    tcase_add_test(tc_is_not_equal, dec_is_not_equal_scale_2);
    suite_add_tcase(s, tc_is_not_equal);
    return s;
}
//

// start is less or equal test

START_TEST(dec_is_less_or_equal_zero) {
    c_decimal a = init();
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_less_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_or_equal_one) {
    c_decimal a = init();
    set_bit(&a, 0);
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_less_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_or_equal_mone) {
    c_decimal a = init();
    set_bit(&a, 0);
    set_sign(&a, 1);
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_less_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_or_equal_inf) {
    c_decimal a = init();
    a.value_type = DEC_INFINITY;
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_less_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_or_equal_neginf) {
    c_decimal a = init();
    a.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_less_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_or_equal_nan) {
    c_decimal a = init();
    a.value_type = DEC_NAN;
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_less_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_or_equal_scale) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 10);
    set_scale(&b, 15);

    int excepted = TRUE;
    int recieved = dec_is_less_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_or_equal_scale_2) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 15);
    set_scale(&b, 20);

    int excepted = TRUE;
    int recieved = dec_is_less_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_less_or_equal_scale_3) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 15);
    set_scale(&b, 20);
    set_sign(&a, 1);
    set_sign(&b, 1);

    int excepted = TRUE;
    int recieved = dec_is_less_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

Suite* is_less_or_equal_suite(void) {
    Suite *s;
    TCase *tc_is_less_or_equal;
    s = suite_create("is_less_or_equal");
    tc_is_less_or_equal = tcase_create("is_less_or_equal");
    tcase_add_test(tc_is_less_or_equal, dec_is_less_or_equal_zero);
    tcase_add_test(tc_is_less_or_equal, dec_is_less_or_equal_one);
    tcase_add_test(tc_is_less_or_equal, dec_is_less_or_equal_one);
    tcase_add_test(tc_is_less_or_equal, dec_is_less_or_equal_mone);
    tcase_add_test(tc_is_less_or_equal, dec_is_less_or_equal_inf);
    tcase_add_test(tc_is_less_or_equal, dec_is_less_or_equal_neginf);
    tcase_add_test(tc_is_less_or_equal, dec_is_less_or_equal_nan);
    tcase_add_test(tc_is_less_or_equal, dec_is_less_or_equal_scale);
    tcase_add_test(tc_is_less_or_equal, dec_is_less_or_equal_scale_2);
    tcase_add_test(tc_is_less_or_equal, dec_is_less_or_equal_scale_3);
    suite_add_tcase(s, tc_is_less_or_equal);
    return s;
}
// end is less or equal test

// start is greater or equal test

START_TEST(dec_is_greater_or_equal_zero) {
    c_decimal a = init();
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_greater_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_or_equal_one) {
    c_decimal a = init();
    set_bit(&a, 0);
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_greater_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_or_equal_mone) {
    c_decimal a = init();
    set_bit(&a, 0);
    set_sign(&a, 1);
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_greater_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_or_equal_inf) {
    c_decimal a = init();
    a.value_type = DEC_INFINITY;
    c_decimal b = init();

    int excepted = TRUE;
    int recieved = dec_is_greater_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_or_equal_neginf) {
    c_decimal a = init();
    a.value_type = DEC_NEGATIVE_INFINITY;
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_greater_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_or_equal_nan) {
    c_decimal a = init();
    a.value_type = DEC_NAN;
    c_decimal b = init();

    int excepted = FALSE;
    int recieved = dec_is_greater_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_or_equal_scale) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 10);
    set_scale(&b, 15);

    int excepted = TRUE;
    int recieved = dec_is_greater_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_or_equal_scale_2) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 15);
    set_scale(&b, 20);

    int excepted = TRUE;
    int recieved = dec_is_greater_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

START_TEST(dec_is_greater_or_equal_scale_3) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 15);
    set_scale(&b, 20);
    set_sign(&a, 1);
    set_sign(&b, 1);

    int excepted = TRUE;
    int recieved = dec_is_greater_or_equal(a, b);
    ck_assert_int_eq(recieved, excepted);
}

Suite* is_greater_or_equal_suite(void) {
    Suite *s;
    TCase *tc_is_greater_or_equal;
    s = suite_create("is_greater_or_equal");
    tc_is_greater_or_equal = tcase_create("is_greater_or_equal");
    tcase_add_test(tc_is_greater_or_equal, dec_is_greater_or_equal_zero);
    tcase_add_test(tc_is_greater_or_equal, dec_is_greater_or_equal_one);
    tcase_add_test(tc_is_greater_or_equal, dec_is_greater_or_equal_one);
    tcase_add_test(tc_is_greater_or_equal, dec_is_greater_or_equal_mone);
    tcase_add_test(tc_is_greater_or_equal, dec_is_greater_or_equal_inf);
    tcase_add_test(tc_is_greater_or_equal, dec_is_greater_or_equal_neginf);
    tcase_add_test(tc_is_greater_or_equal, dec_is_greater_or_equal_nan);
    tcase_add_test(tc_is_greater_or_equal, dec_is_greater_or_equal_scale);
    tcase_add_test(tc_is_greater_or_equal, dec_is_greater_or_equal_scale_2);
    tcase_add_test(tc_is_greater_or_equal, dec_is_greater_or_equal_scale_3);
    suite_add_tcase(s, tc_is_greater_or_equal);
    return s;
}
// end is greater or equal test

// end equal operators section

// converters section

// start from decimal to int test

START_TEST(from_decimal_to_int_zero) {
    int dst = 0;
    c_decimal val = init();
    from_decimal_to_int(val, &dst);
    int except = 0;

    ck_assert_int_eq(dst, except);
}

START_TEST(from_decimal_to_int_one) {
    int dst = 0;
    c_decimal val = init();
    val.bits[LOW] = 1;
    from_decimal_to_int(val, &dst);
    int except = 1;

    ck_assert_int_eq(dst, except);
}

START_TEST(from_decimal_to_int_trunc) {
    int dst = 0;
    c_decimal val = init();
    val.bits[LOW] = 123456;
    set_scale(&val, 3);
    from_decimal_to_int(val, &dst);
    int except = 123;

    ck_assert_int_eq(dst, except);
}

START_TEST(from_decimal_to_int_overflow1) {
    int dst = 0;
    c_decimal val = init();
    val.bits[LOW] = 10;
    val.bits[MID] = 10;
    int code = from_decimal_to_int(val, &dst);

    ck_assert_int_eq(code, CALCULATION_ERROR);
}

START_TEST(from_decimal_to_int_overflow2) {
    int dst = 0;
    c_decimal val = init();
    val.bits[LOW] = -1;
    int code = from_decimal_to_int(val, &dst);

    ck_assert_int_eq(code, CALCULATION_ERROR);
}

START_TEST(from_decimal_to_int_mone) {
    int dst = 0;
    c_decimal val = init();
    set_bit(&val, 0);
    set_sign(&val, 1);
    from_decimal_to_int(val, &dst);
    int except = -1;

    ck_assert_int_eq(dst, except);
}

START_TEST(from_decimal_to_int_inf) {
    int dst = 0;
    c_decimal val = init();
    val.value_type = DEC_INFINITY;
    int actual = from_decimal_to_int(val, &dst);
    int except = CALCULATION_ERROR;

    ck_assert_int_eq(actual, except);
}

START_TEST(from_decimal_to_int_neg_inf) {
    int* dst = NULL;
    c_decimal val = init();
    val.value_type = DEC_NEGATIVE_INFINITY;
    int actual = from_decimal_to_int(val, dst);
    int except = CALCULATION_ERROR;

    ck_assert_int_eq(actual, except);
}

START_TEST(from_decimal_to_convertingerror2) {
    int* dst = NULL;
    c_decimal val = init();
    val.value_type = DEC_NEGATIVE_INFINITY;
    int actual = from_decimal_to_int(val, NULL);
    int except = CALCULATION_ERROR;

    ck_assert_int_eq(actual, except);
}

Suite* from_decimal_to_int_suite(void) {
    Suite *s;
    TCase *tc_from_decimal_to_int;
    s = suite_create("from_decimal_to_int");
    tc_from_decimal_to_int = tcase_create("from_decimal_to_int");
    tcase_add_test(tc_from_decimal_to_int, from_decimal_to_int_zero);
    tcase_add_test(tc_from_decimal_to_int, from_decimal_to_int_one);
    tcase_add_test(tc_from_decimal_to_int, from_decimal_to_int_mone);
    tcase_add_test(tc_from_decimal_to_int, from_decimal_to_int_inf);
    tcase_add_test(tc_from_decimal_to_int, from_decimal_to_int_trunc);
    tcase_add_test(tc_from_decimal_to_int, from_decimal_to_int_neg_inf);
    tcase_add_test(tc_from_decimal_to_int, from_decimal_to_convertingerror2);
    tcase_add_test(tc_from_decimal_to_int, from_decimal_to_int_overflow1);
    tcase_add_test(tc_from_decimal_to_int, from_decimal_to_int_overflow2);
    suite_add_tcase(s, tc_from_decimal_to_int);
    return s;
}

// end from decimal to int test

// start another functions secion

// start dec_truncate test

START_TEST(dec_truncate_test) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 23);

    a = dec_truncate(a);

    int scale_a = get_scale(a);
    int scale_b = get_scale(b);
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(dec_truncate_test1) {
    c_decimal a = init();

    a.bits[LOW] = 12349847;
    set_scale(&a, 4);
    set_sign(&a, 1);
    a = dec_truncate(a);

    ck_assert_int_eq(a.bits[LOW], 1234);
    ck_assert_int_eq(get_sign(a), 1);
}

START_TEST(dec_truncate_test_inf) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_INFINITY;
    b.value_type = DEC_INFINITY;

    a = dec_truncate(a);

    int scale_a = dec_is_equal(a, b);
    int scale_b = TRUE;
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(dec_truncate_test_neginf) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_NEGATIVE_INFINITY;
    b.value_type = DEC_NEGATIVE_INFINITY;

    a = dec_truncate(a);

    int scale_a = dec_is_equal(a, b);
    int scale_b = TRUE;
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(dec_truncate_test_nan) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_NAN;
    b.value_type = DEC_NAN;

    a = dec_truncate(a);

    int scale_a = dec_is_equal(a, b);
    int scale_b = FALSE;
    ck_assert_int_eq(scale_a, scale_b);
}

Suite* truncate_suite(void) {
    Suite *s;
    TCase *tc_truncate;
    s = suite_create("truncate");
    tc_truncate = tcase_create("truncate");

    tcase_add_test(tc_truncate, dec_truncate_test);
    tcase_add_test(tc_truncate, dec_truncate_test1);
    tcase_add_test(tc_truncate, dec_truncate_test_inf);
    tcase_add_test(tc_truncate, dec_truncate_test_neginf);
    tcase_add_test(tc_truncate, dec_truncate_test_nan);

    suite_add_tcase(s, tc_truncate);

    return s;
}

// end dec_truncate test

// start dec_floor test

START_TEST(dec_floor_test) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 23);

    a = dec_floor(a);

    int scale_a = get_scale(a);
    int scale_b = get_scale(b);
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(dec_floor_test_two) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 1);
    set_bit(&a, 1);
    set_bit(&a, 5);
    set_bit(&a, 4);
    set_bit(&a, 2);
    set_bit(&a, 0);
    set_bit(&b, 1);
    set_bit(&b, 2);

    set_sign(&b, 1);
    set_sign(&a, 1);

    c_decimal res = dec_floor(a);

    int scale_a = dec_is_equal(res, b);
    int scale_b = TRUE;
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(dec_floor_test_third) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 1);
    set_bit(&a, 1);
    set_bit(&a, 5);
    set_bit(&a, 4);
    set_bit(&a, 2);
    set_bit(&a, 0);
    set_bit(&b, 0);
    set_bit(&b, 2);

    set_sign(&b, 0);
    set_sign(&a, 0);

    c_decimal res = dec_floor(a);

    int scale_a = dec_is_equal(res, b);
    int scale_b = TRUE;
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(floor_test) {
    c_decimal a = init();
    a.bits[LOW] = 1234624;

    set_scale(&a, 3);
    a = dec_floor(a);
    int og[4];
    og[LOW] = 1234;
    og[MID] = 0;
    og[HIGH] = 0;
    og[SCALE] = 0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(a.bits[i], og[i]);
    }
    ck_assert_int_eq(a.value_type, DEC_NORMAL_VALUE);
}

START_TEST(floor_test2) {
    c_decimal a = init();
    a.bits[LOW] = 1234999;
    set_scale(&a, 3);
    a = dec_floor(a);
    int og[4];
    og[LOW] = 1234;
    og[MID] = 0;
    og[HIGH] = 0;
    og[SCALE] = 0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(a.bits[i], og[i]);
    }
    ck_assert_int_eq(a.value_type, DEC_NORMAL_VALUE);
}

START_TEST(floor_test3) {
    c_decimal a = init();
    a.bits[LOW] = 365148;
    a.bits[MID] = 132;
    a.bits[HIGH] = 1;
    set_scale(&a, 5);
    set_sign(&a, 1);
    a = dec_floor(a);
    int og[4];
    og[LOW]   = 0xac9d9d38;
    og[MID]   = 0xa7c5;
    og[HIGH]  = 0x0;
    og[SCALE] = 0x80000000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(a.bits[i], og[i]);
    }
    ck_assert_int_eq(a.value_type, DEC_NORMAL_VALUE);
}

START_TEST(floor_neg_test) {
    c_decimal a = init();
    a.bits[LOW] = 123414;
    set_scale(&a, 2);
    set_sign(&a, 1);
    a = dec_floor(a);
    int og[4];
    og[LOW] = 1235;
    og[MID] = 0;
    og[HIGH] = 0;
    og[SCALE] = 0x80000000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(a.bits[i], og[i]);
    }
    ck_assert_int_eq(a.value_type, DEC_NORMAL_VALUE);
}

START_TEST(floor_int_test) {
    c_decimal a = init();
    a.bits[LOW] = 123414;
    a.bits[HIGH] = 123;
    a = dec_floor(a);
    int og[4];
    og[LOW] = 123414;
    og[MID] = 0;
    og[HIGH] = 123;
    og[SCALE] = 0;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(a.bits[i], og[i]);
    }
    ck_assert_int_eq(a.value_type, DEC_NORMAL_VALUE);
}

START_TEST(floor_neg_int_test) {
    c_decimal a = init();
    a.bits[LOW] = 65148;
    a.bits[MID] = 1234;
    a.bits[HIGH] = 4;
    set_sign(&a, 1);
    a = dec_floor(a);
    int og[4];
    og[LOW] = 65148;
    og[MID] = 1234;
    og[HIGH] = 4;
    og[SCALE] = 0x80000000;
    for (int i = 0; i <= SCALE; i++) {
        ck_assert_int_eq(a.bits[i], og[i]);
    }
    ck_assert_int_eq(a.value_type, DEC_NORMAL_VALUE);
}

START_TEST(floor_nan_test) {
    c_decimal a = init();
    a.value_type = DEC_NAN;
    a = dec_floor(a);
    ck_assert_int_eq(a.value_type, DEC_NAN);
}

START_TEST(floor_inf_test) {
    c_decimal a = init();
    a.value_type = DEC_INFINITY;
    a = dec_floor(a);
    ck_assert_int_eq(a.value_type, DEC_INFINITY);
}

START_TEST(floor_neg_inf_test) {
    c_decimal a = init();
    a.value_type = DEC_NEGATIVE_INFINITY;
    a = dec_floor(a);
    ck_assert_int_eq(a.value_type, DEC_NEGATIVE_INFINITY);
}

Suite* floor_suite(void) {
    Suite *s;
    TCase *tc_floor;
    s = suite_create("floor");
    tc_floor = tcase_create("floor");

    tcase_add_test(tc_floor, dec_floor_test);
    tcase_add_test(tc_floor, dec_floor_test_two);
    tcase_add_test(tc_floor, dec_floor_test_third);
    tcase_add_test(tc_floor, floor_test);
    tcase_add_test(tc_floor, floor_test2);
    tcase_add_test(tc_floor, floor_test3);
    tcase_add_test(tc_floor, floor_neg_test);
    tcase_add_test(tc_floor, floor_int_test);
    tcase_add_test(tc_floor, floor_neg_int_test);
    tcase_add_test(tc_floor, floor_nan_test);
    tcase_add_test(tc_floor, floor_inf_test);
    tcase_add_test(tc_floor, floor_neg_inf_test);

    suite_add_tcase(s, tc_floor);

    return s;
}

// end dec_floor test

// start round test

START_TEST(dec_round_test) {
    c_decimal a = init();
    c_decimal b = init();
    set_scale(&a, 23);

    a = dec_round(a);

    int scale_a = get_scale(a);
    int scale_b = get_scale(b);
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(dec_round_test_3) {
    c_decimal a = init();
    c_decimal b = init();
    set_bit(&a, 0);  // 55
    set_bit(&a, 5);
    set_bit(&a, 4);
    set_bit(&a, 2);
    set_bit(&a, 1);

    set_scale(&a, 1);
    b.bits[LOW] = 5;

    c_decimal res = dec_round(a);

    int scale_a = dec_is_equal(res, b);
    int scale_b = TRUE;
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(dec_round_test_2) {
    c_decimal a = init();
    c_decimal b = init();
    set_bit(&a, 5);
    set_bit(&a, 4);
    set_bit(&a, 2);
    set_bit(&a, 1);

    set_scale(&a, 1);

    set_bit(&b, 0);
    set_bit(&b, 2);

    c_decimal res = dec_round(a);

    int scale_a = dec_is_equal(res, b);
    int scale_b = TRUE;
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(dec_round_test_nan) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_NAN;
    b.value_type = DEC_NAN;

    c_decimal res = dec_round(a);

    int scale_a = dec_is_equal(res, b);
    int scale_b = FALSE;
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(dec_round_test_inf) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_INFINITY;
    b.value_type = DEC_INFINITY;

    c_decimal res = dec_round(a);

    int scale_a = dec_is_equal(res, b);
    int scale_b = TRUE;
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(dec_round_test_neginf) {
    c_decimal a = init();
    c_decimal b = init();
    a.value_type = DEC_NEGATIVE_INFINITY;
    b.value_type = DEC_NEGATIVE_INFINITY;

    c_decimal res = dec_round(a);

    int scale_a = dec_is_equal(res, b);
    int scale_b = TRUE;
    ck_assert_int_eq(scale_a, scale_b);
}

START_TEST(round_test1) {
    c_decimal a = init();
    a.bits[LOW] = 1235745;
    set_scale(&a, 3);
    a = dec_round(a);
    int og[4];
    og[LOW] = 1236;
    og[MID] = 0;
    og[HIGH] = 0;
    og[SCALE] = 0;

    for (int i = 0; i < SCALE; i++) {
        ck_assert_int_eq(a.bits[i], og[i]);
    }
    ck_assert_int_eq(a.value_type, DEC_NORMAL_VALUE);
}

START_TEST(round_test2) {
    c_decimal a = init();
    a.bits[LOW] = 948999;
    set_scale(&a, 3);
    a = dec_round(a);
    int og[4];
    og[LOW] = 949;
    og[MID] = 0;
    og[HIGH] = 0;
    og[SCALE] = 0;

    for (int i = 0; i < SCALE; i++) {
        ck_assert_int_eq(a.bits[i], og[i]);
    }
    ck_assert_int_eq(a.value_type, DEC_NORMAL_VALUE);
}

START_TEST(round_test3) {
    c_decimal a = init();
    a.bits[LOW] = 65158;
    set_scale(&a, 2);
    a = dec_round(a);
    int og[4];
    og[LOW] = 651;
    og[MID] = 0;
    og[HIGH] = 0;
    og[SCALE] = 0;

    for (int i = 0; i < SCALE; i++) {
        ck_assert_int_eq(a.bits[i], og[i]);
    }
    ck_assert_int_eq(a.value_type, DEC_NORMAL_VALUE);
}

START_TEST(round_neg_test1) {
    c_decimal a = init();
    a.bits[LOW] = 6591487;
    set_scale(&a, 2);
    set_sign(&a, 1);
    a = dec_round(a);
    int og[4];
    og[LOW] = 65915;
    og[MID] = 0;
    og[HIGH] = 0;
    og[SCALE] = 0x80000000;

    for (int i = 0; i < SCALE; i++) {
        ck_assert_int_eq(a.bits[i], og[i]);
    }
    ck_assert_int_eq(a.value_type, DEC_NORMAL_VALUE);
}

Suite* round_suite(void) {
    Suite *s;
    TCase *tc_round;
    s = suite_create("round");
    tc_round = tcase_create("round");

    tcase_add_test(tc_round, dec_round_test);
    tcase_add_test(tc_round, dec_round_test_2);
    tcase_add_test(tc_round, dec_round_test_3);
    tcase_add_test(tc_round, dec_round_test_inf);
    tcase_add_test(tc_round, dec_round_test_neginf);
    tcase_add_test(tc_round, dec_round_test_nan);
    tcase_add_test(tc_round, round_test1);
    tcase_add_test(tc_round, round_test2);
    tcase_add_test(tc_round, round_neg_test1);

    suite_add_tcase(s, tc_round);
    return s;
}


// end round test

// end another function convertion

START_TEST(d2float_int) {
    c_decimal src = init();
    src.bits[LOW] = 6235;
    float res;
    float exp = 6235;
    int code = from_decimal_to_float(src, &res);
    ck_assert_int_eq(code, SUCCESS);
    ck_assert_float_eq(res, exp);
}

START_TEST(d2float_1) {
    c_decimal src = init();
    src.bits[LOW] = 796245;
    set_scale(&src, 3);
    set_sign(&src, 1);
    float res;
    float exp = -796.245;
    int code = from_decimal_to_float(src, &res);
    ck_assert_int_eq(code, SUCCESS);
    ck_assert_float_eq(res, exp);
}

START_TEST(d2float_2) {
    c_decimal src = init();
    src.bits[LOW] = 7962;
    set_scale(&src, 1);
    float res;
    float exp = 796.2;
    int code = from_decimal_to_float(src, &res);
    ck_assert_int_eq(code, SUCCESS);
    ck_assert_float_eq(res, exp);
}

START_TEST(d2float_3) {
    c_decimal src = init();
    set_bit(&src, 32);
    float res;
    float exp = 4294967296;
    int code = from_decimal_to_float(src, &res);
    ck_assert_int_eq(code, SUCCESS);
    ck_assert_float_eq(res, exp);
}

START_TEST(d2float_inf) {
    c_decimal src = init();
    src.value_type = DEC_INFINITY;
    float res;
    float exp = INFINITY;
    int code = from_decimal_to_float(src, &res);
    ck_assert_int_eq(code, SUCCESS);
    ck_assert_float_eq(res, exp);
}

START_TEST(d2float_neg_inf) {
    c_decimal src = init();
    src.value_type = DEC_NEGATIVE_INFINITY;
    float res;
    float exp = -INFINITY;
    int code = from_decimal_to_float(src, &res);
    ck_assert_int_eq(code, SUCCESS);
    ck_assert_float_eq(res, exp);
}

START_TEST(d2float_nan) {
    c_decimal src = init();
    src.value_type = DEC_NAN;
    float res;
    int code = from_decimal_to_float(src, &res);
    ck_assert_int_eq(code, SUCCESS);
    ck_assert_float_nan(res);
}

START_TEST(d2float_null) {
    c_decimal src = init();
    int code = from_decimal_to_float(src, NULL);
    ck_assert_int_eq(code, CALCULATION_ERROR);
}

Suite *decimal_to_float_s(void) {
    Suite *s;
    TCase *tc;
    s = suite_create("decimal_to_float");
    tc = tcase_create("decimal_to_float");

    tcase_add_test(tc, d2float_int);
    tcase_add_test(tc, d2float_1);
    tcase_add_test(tc, d2float_2);
    tcase_add_test(tc, d2float_3);
    tcase_add_test(tc, d2float_inf);
    tcase_add_test(tc, d2float_neg_inf);
    tcase_add_test(tc, d2float_null);

    suite_add_tcase(s, tc);

    return s;
}

int main() {
    Suite *add_m = add_suite();
    Suite *negate_m = negate_suite();
    Suite *sub_m = sub_suite();
    Suite *mul_m = mul_suite();
    Suite *float_to_decimal_m = float_to_decimal_suite();
    Suite *int_to_decimal_m = int_to_decimal_suite();
    Suite *is_equal_suite_m = is_equal_suite();
    Suite *is_less_suite_m = is_less_suite();
    Suite *is_greater_suite_m = is_greater_suite();
    Suite *is_not_equal_suite_m = is_not_equal_suite();
    Suite *is_less_or_equal_suite_m = is_less_or_equal_suite();
    Suite *is_greater_or_equal_suite_m = is_greater_or_equal_suite();
    Suite *from_decimal_to_int_suite_m = from_decimal_to_int_suite();
    Suite *truncate_suite_m = truncate_suite();
    Suite *floor_suite_m = floor_suite();
    Suite *round_suite_m = round_suite();
    Suite *mod_suite_m = mod_suite();
    Suite *div_suite_m = div_suite();
    Suite *decimal_to_float_m = decimal_to_float_s();

    SRunner* sr = srunner_create(add_m);
    srunner_add_suite(sr, negate_m);
    srunner_add_suite(sr, sub_m);
    srunner_add_suite(sr, mul_m);
    srunner_add_suite(sr, float_to_decimal_m);
    srunner_add_suite(sr, int_to_decimal_m);
    srunner_add_suite(sr, is_equal_suite_m);
    srunner_add_suite(sr, is_less_suite_m);
    srunner_add_suite(sr, is_greater_suite_m);
    srunner_add_suite(sr, is_not_equal_suite_m);
    srunner_add_suite(sr, is_less_or_equal_suite_m);
    srunner_add_suite(sr, is_greater_or_equal_suite_m);
    srunner_add_suite(sr, from_decimal_to_int_suite_m);
    srunner_add_suite(sr, truncate_suite_m);
    srunner_add_suite(sr, floor_suite_m);
    srunner_add_suite(sr, round_suite_m);
    srunner_add_suite(sr, mod_suite_m);
    srunner_add_suite(sr, div_suite_m);
    srunner_add_suite(sr, decimal_to_float_m);

    srunner_run_all(sr, CK_NORMAL);
    srunner_free(sr);
    return 0;
}
