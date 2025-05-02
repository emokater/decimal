#include <check.h>
#include <stdlib.h>
#include "s21_decimal.h"
#include "s21_decimal.c"

// s21_is_valid_decimal
// s21_decimal_zero
// s21_get_scale(value);

// START_TEST(test_s21_trim_trailing_zeros) {
//     // 1: 100.0 → 10
//     s21_decimal value1 = {{100, 0, 0, 0}};
//     s21_set_scale(&value1, 1);
//     s21_decimal expected1 = {{10, 0, 0, 0}};
//     s21_set_scale(&expected1, 0);
//     s21_decimal result1 = s21_trim_trailing_zeros(value1);
//     ck_assert_int_eq(result1.bits[0], expected1.bits[0]);
//     ck_assert_int_eq(s21_get_scale(result1), s21_get_scale(expected1));

//     // 2: 123 → 123
//     s21_decimal value2 = {{123, 0, 0, 0}};
//     s21_set_scale(&value2, 0);
//     s21_decimal result2 = s21_trim_trailing_zeros(value2);
//     ck_assert_int_eq(result2.bits[0], value2.bits[0]);
//     ck_assert_int_eq(s21_get_scale(result2), s21_get_scale(value2));

//     // 3: 123.000 → 123
//     s21_decimal value3 = {{123000, 0, 0, 0}};
//     s21_set_scale(&value3, 3);
//     s21_decimal expected3 = {{123, 0, 0, 0}};
//     s21_set_scale(&expected3, 0);
//     s21_decimal result3 = s21_trim_trailing_zeros(value3);
//     ck_assert_int_eq(result3.bits[0], expected3.bits[0]);
//     ck_assert_int_eq(s21_get_scale(result3), s21_get_scale(expected3));

//     // Тест 4: Число с дробной частью и нулями (123.450 → 123.45)
//     s21_decimal value4 = {{123450, 0, 0, 0}};
//     s21_set_scale(&value4, 3);
//     s21_decimal expected4 = {{12345, 0, 0, 0}};
//     s21_set_scale(&expected4, 2);
//     s21_decimal result4 = s21_trim_trailing_zeros(value4);
//     ck_assert_int_eq(result4.bits[0], expected4.bits[0]);
//     ck_assert_int_eq(s21_get_scale(result4), s21_get_scale(expected4));

//     // Тест 5: Число равно нулю (0.00000 → 0)
//     s21_decimal value5 = {{0, 0, 0, 0}};
//     s21_set_scale(&value5, 5);
//     s21_decimal expected5 = {{0, 0, 0, 0}};
//     s21_set_scale(&expected5, 0);
//     s21_decimal result5 = s21_trim_trailing_zeros(value5);
//     ck_assert_int_eq(result5.bits[0], expected5.bits[0]);
//     ck_assert_int_eq(s21_get_scale(result5), s21_get_scale(expected5));

//     // Тест 6: Максимальное возможное число (без изменений)
//     s21_decimal value6 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
//     s21_set_scale(&value6, 5);
//     s21_decimal expected6 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
//     s21_set_scale(&expected6, 0);
//     s21_decimal result6 = s21_trim_trailing_zeros(value6);
//     ck_assert_int_eq(result6.bits[0], expected6.bits[0]);
//     ck_assert_int_eq(result6.bits[1], expected6.bits[1]);
//     ck_assert_int_eq(result6.bits[2], expected6.bits[2]);
//     ck_assert_int_eq(s21_get_scale(result6), s21_get_scale(expected6));
// }
// END_TEST

// ADD

START_TEST(test_s21_add_basic) {
       s21_decimal a = {{3333333330, 0, 0, 0}};
       s21_decimal b = {{3, 0, 0, 0}};
       s21_decimal result = {{0, 0, 0, 0}};

       int res = s21_add(a, b, &result);

       ck_assert_int_eq(res, 0);
       ck_assert_int_eq(result.bits[0], 3333333333);
}
END_TEST

START_TEST(test_s21_add_overflow) {
   s21_decimal a = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
   s21_decimal b = {{UINT32_MAX, 0, 0, 0}};
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_add(a, b, &result);

   ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_add_positive) {
    s21_decimal a = {{1, 0, 0, 0}};
    s21_decimal b = {{2, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_add(a, b, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(result.bits[0], 3);
}
END_TEST

START_TEST(test_s21_add_negative) {
    s21_decimal a = {{5, 0, 0, 0x80000000}};
    s21_decimal b = {{10, 0, 0, 0x80000000}};
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_add(a, b, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(result.bits[0], 15);
    ck_assert_int_eq(result.bits[3], 0x80000000);
}
END_TEST

START_TEST(test_s21_add_opposite_numbers) {
    s21_decimal a = {{123456789, 0, 0, 0}};
    s21_decimal b = {{123456789, 0, 0, 0x80000000}};
    s21_decimal result = {{1, 1, 1, 1}};

    int res = s21_add(a, b, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(result.bits[0], 0);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
    ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_add_with_zero) {
    s21_decimal a = {{42, 0, 0, 0}};
    s21_decimal b = {{0, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_add(a, b, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(result.bits[0], 42);
}
END_TEST

START_TEST(test_s21_add_negative_overflow) {
    s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    s21_decimal b = {{1, 0, 0, 0x80000000}};
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_add(a, b, &result);

    ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(test_s21_add_complex_case) {
    s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}; // -максимальное число
    s21_decimal b = {{1, 0, 0, 0x80000000}}; // -1
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_add(a, b, &result);

    ck_assert_int_eq(res, 2); // Ожидаем ошибку переполнения
}
END_TEST

// SUB

START_TEST(test_s21_sub_basic) {
   s21_decimal a = {{5, 0, 0, 0}};
   s21_decimal b = {{3, 0, 0, 0}};
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_sub(a, b, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 2);
}
END_TEST

START_TEST(test_s21_sub_underflow) {
   s21_decimal a = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 1<<31}};
   s21_decimal b = {{1, 0, 0, 1}};
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_sub(a, b, &result);

   ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(test_s21_sub_negative_result) {
   s21_decimal a = {{3, 0, 0, 0}};
   s21_decimal b = {{5, 0, 0, 0}};
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_sub(a, b, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 2);
   ck_assert_int_eq((uint32_t) result.bits[3] & (1 << 31), (uint32_t) (1 << 31));
}
END_TEST

START_TEST(test_s21_sub_large_numbers) {
    s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal b = {{1, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_sub(a, b, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(result.bits[0], 0xFFFFFFFE);
    ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[2], 0xFFFFFFFF);
}
END_TEST

START_TEST(test_s21_sub_same_numbers) {
    s21_decimal a = {{123456789, 0, 0, 0}};
    s21_decimal b = {{123456789, 0, 0, 0}};
    s21_decimal result = {{1, 1, 1, 1}};

    int res = s21_sub(a, b, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(result.bits[0], 0);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
    ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_sub_negative_zero) {
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{5, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0x80000000}};

    int res = s21_sub(a, b, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(result.bits[0], 0);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
    ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_sub_overflow_negative) {
    s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    s21_decimal b = {{1, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_sub(a, b, &result);

    ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(test_s21_sub_carry_propagation) {
    s21_decimal a = {{0, 0, 1, 0}};
    s21_decimal b = {{1, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_sub(a, b, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

// MUL

START_TEST(test_s21_mul_basic) {
   s21_decimal a = {{2, 0, 0, 0}};
   s21_decimal b = {{3, 0, 0, 0}};
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_mul(a, b, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 6);
}
END_TEST

START_TEST(test_s21_mul_overflow) {
   s21_decimal a = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, }};
   s21_decimal b = {{2, 0, 0, 0}};
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_mul(a, b, &result);

   ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_mul_negative_result) {
   s21_decimal a = {{2, 0, 0, 0}};
   s21_decimal b = {{3, 0, 0, (1 << 31)}};
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_mul(a, b, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 6);
   ck_assert_int_eq((uint32_t) result.bits[3] & (1 << 31), (uint32_t) (1 << 31));
}
END_TEST

//

START_TEST(test_s21_mul_large_numbers) {
    s21_decimal a = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
    s21_decimal b = {{2, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_mul(a, b, &result);

    ck_assert_int_eq(res, 1);  
}
END_TEST

START_TEST(test_s21_mul_zero) {
    s21_decimal a = {{123456789, 0, 0, 0}};
    s21_decimal b = {{0, 0, 0, 0}};
    s21_decimal result = {{1, 1, 1, 1}};

    int res = s21_mul(a, b, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(result.bits[0], 0);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
    ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_mul_negative_negative) {
    s21_decimal a = {{2, 0, 0, 0x80000000}};
    s21_decimal b = {{3, 0, 0, 0x80000000}};
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_mul(a, b, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(result.bits[0], 6);
    ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_mul_scale_handling) {
    s21_decimal a = {{1, 0, 0, 0}};
    s21_decimal b = {{1, 0, 0, 0}};
    s21_set_scale(&a, 14);
    s21_set_scale(&b, 14);
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_mul(a, b, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(s21_get_scale(result), 28);
}
END_TEST

// ОШИБКА но возможно переполнения быть не должно
START_TEST(test_s21_mul_carry_propagation) {
    s21_decimal a = {{0xFFFFFFFF, 0, 0, 0}};
    s21_decimal b = {{0xFFFFFFFF, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};

    int res = s21_mul(a, b, &result);

    ck_assert_int_eq(res, 1);
}
END_TEST

// DIV

START_TEST(test_s21_div_basic) {
   s21_decimal a = {{6, 0, 0, 0}};
   s21_decimal b = {{2, 0, 0, 0}};
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_div(a, b, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 3);
}
END_TEST

START_TEST(test_s21_div_by_zero) {
   s21_decimal a = {{5, 0, 0, 0}};
   s21_decimal b = {{0, 0, 0, 0}};
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_div(a, b, &result);

   ck_assert_int_eq(res, 3);
}
END_TEST

START_TEST(test_s21_div_negative_result) {
   s21_decimal a = {{6, 0, 0, 0}};
   s21_decimal b = {{2, 0, 0, 1 << 31}};
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_div(a, b, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 3);
   ck_assert_int_eq((uint32_t) result.bits[3] & (1 << 31), (uint32_t) (1 << 31));
}
END_TEST

// is_less

START_TEST(test_s21_is_less_basic) {
   s21_decimal a = {{1, 0, 0, 0}};
   s21_decimal b = {{2, 0, 0, 0}};

   int res = s21_is_less(a, b);

   ck_assert_int_eq(res, 1);
}
END_TEST

//

START_TEST(test_s21_is_less_equal_numbers) {
   s21_decimal a = {{1, 0, 0, 0}};
   s21_decimal b = {{1, 0, 0, 0}};
   int res = s21_is_less(a, b);
   ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_negative_numbers) {
   s21_decimal a = {{1, 0, 0, 0x80000000}};
   s21_decimal b = {{2, 0, 0, 0x80000000}};
   int res = s21_is_less(a, b);
   ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_positive_negative) {
   s21_decimal a = {{1, 0, 0, 0}};
   s21_decimal b = {{2, 0, 0, 0x80000000}};
   int res = s21_is_less(a, b);
   ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_negative_positive) {
   s21_decimal a = {{2, 0, 0, 0x80000000}};
   s21_decimal b = {{1, 0, 0, 0}};
   int res = s21_is_less(a, b);
   ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_large_numbers) {
   s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF, 0}};
   s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
   int res = s21_is_less(a, b);
   ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_min_max) {
   s21_decimal a = {{0, 0, 0, 0x80000000}};
   s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
   int res = s21_is_less(a, b);
   ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_zero) {
   s21_decimal a = {{0, 0, 0, 0}};
   s21_decimal b = {{0, 0, 0, 0}};
   int res = s21_is_less(a, b);
   ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_zero_negative) {
   s21_decimal a = {{0, 0, 0, 0}};
   s21_decimal b = {{0, 0, 0, 0x80000000}};
   int res = s21_is_less(a, b);
   ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_equal) {
   s21_decimal a = {{1, 0, 0, 0}};
   s21_decimal b = {{1, 0, 0, 0}};

   int res = s21_is_less_or_equal(a, b);

   ck_assert_int_eq(res, 1);
}
END_TEST

//

START_TEST(test_s21_is_less_or_equal_positive_negative) {
   s21_decimal a = {{5, 0, 0, 0}};
   s21_decimal b = {{5, 0, 0, 0x80000000}};
   int res = s21_is_less_or_equal(a, b);
   ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_negative_numbers) {
   s21_decimal a = {{2, 0, 0, 0x80000000}};
   s21_decimal b = {{1, 0, 0, 0x80000000}};
   int res = s21_is_less_or_equal(a, b);
   ck_assert_int_eq(res, 1);
}
END_TEST

//

START_TEST(test_s21_is_greater) {
   s21_decimal a = {{3, 0, 0, 0}};
   s21_decimal b = {{2, 0, 0, 0}};

   int res = s21_is_greater(a, b);

   ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_equal_numbers) {
    s21_decimal a = {{12345, 0, 0, 0}};
    s21_decimal b = {{12345, 0, 0, 0}};

    int res = s21_is_greater(a, b);

    ck_assert_int_eq(res, 0);  // a == b, должно быть 0
}
END_TEST

START_TEST(test_s21_is_greater_negative_vs_positive) {
    s21_decimal a = {{3, 0, 0, 0x80000000}};
    s21_decimal b = {{2, 0, 0, 0}};

    int res = s21_is_greater(a, b);

    ck_assert_int_eq(res, 0);  // Отрицательное число не может быть больше положительного
}
END_TEST

START_TEST(test_s21_is_greater_positive_vs_negative) {
    s21_decimal a = {{3, 0, 0, 0}};
    s21_decimal b = {{2, 0, 0, 0x80000000}};

    int res = s21_is_greater(a, b);

    ck_assert_int_eq(res, 1);  // Положительное число всегда больше отрицательного
}
END_TEST

START_TEST(test_s21_is_greater_negative_numbers) {
    s21_decimal a = {{3, 0, 0, 0x80000000}};
    s21_decimal b = {{5, 0, 0, 0x80000000}};

    int res = s21_is_greater(a, b);

    ck_assert_int_eq(res, 1);  // -3 > -5 должно быть 1
}
END_TEST

START_TEST(test_s21_is_greater_large_numbers) {
    s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal b = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};

    int res = s21_is_greater(a, b);

    ck_assert_int_eq(res, 1);  // MAX_DECIMAL > MAX_DECIMAL - 1
}
END_TEST

START_TEST(test_s21_is_greater_scale_difference) {
    s21_decimal a = {{1000, 0, 0, 0}};
    s21_decimal b = {{1, 0, 0, 0}};
    s21_set_scale(&a, 3);

    int res = s21_is_greater(a, b);

    ck_assert_int_eq(res, 0);  // 1000 == 1.000 после нормализации
}
END_TEST

START_TEST(test_s21_is_greater_or_equal) {
   s21_decimal a = {{3, 0, 0, 0}};
   s21_decimal b = {{2, 0, 0, 0}};

   int res = s21_is_greater_or_equal(a, b);

   ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_equal_numbers) {
   s21_decimal a = {{3, 0, 0, 0}};
   s21_decimal b = {{3, 0, 0, 0}};
   int res = s21_is_greater_or_equal(a, b);
   ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_negative_numbers) {
   s21_decimal a = {{3, 0, 0, 0x80000000}};
   s21_decimal b = {{2, 0, 0, 0x80000000}};
   int res = s21_is_greater_or_equal(a, b);
   ck_assert_int_eq(res, 0);
}
END_TEST



//



//

START_TEST(test_s21_is_equal) {
   s21_decimal a = {{5, 0, 0, 0}};
   s21_decimal b = {{5, 0, 0, 0}};

   int res = s21_is_equal(a, b);

   ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_equal_different_numbers) {
   s21_decimal a = {{5, 0, 0, 0}};
   s21_decimal b = {{6, 0, 0, 0}};
   int res = s21_is_equal(a, b);
   ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_equal_negative_numbers) {
   s21_decimal a = {{5, 0, 0, 0x80000000}};
   s21_decimal b = {{5, 0, 0, 0x80000000}};
   int res = s21_is_equal(a, b);
   ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_equal_positive_negative) {
   s21_decimal a = {{5, 0, 0, 0}};
   s21_decimal b = {{5, 0, 0, 0x80000000}};
   int res = s21_is_equal(a, b);
   ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_equal_large_numbers) {
   s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
   s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
   int res = s21_is_equal(a, b);
   ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_equal_zero) {
   s21_decimal a = {{0, 0, 0, 0}};
   s21_decimal b = {{0, 0, 0, 0}};
   int res = s21_is_equal(a, b);
   ck_assert_int_eq(res, 1);
}
END_TEST

//

START_TEST(test_s21_is_not_equal) {
   s21_decimal a = {{5, 0, 0, 0}};
   s21_decimal b = {{3, 0, 0, 0}};

   int res = s21_is_not_equal(a, b);

   ck_assert_int_eq(res, 1);
}
END_TEST

// START_TEST(test_s21_floor) {
//    s21_decimal a = {{314, 0, 0, (2 << 16)}};
//    s21_decimal result = {{0, 0, 0, 0}};

//    int res = s21_floor(a, &result);

//    ck_assert_int_eq(res, 0);
//    ck_assert_int_eq(result.bits[0], 3);
// }
// END_TEST

// START_TEST(test_s21_round) {
//    s21_decimal a = {{314, 0, 0, (2 << 16)}};
//    s21_decimal result = {{0, 0, 0, 0}};

//    int res = s21_round(a, &result);

//    ck_assert_int_eq(res, 0);
//    ck_assert_int_eq(result.bits[0], 3);
// }
// END_TEST

// START_TEST(test_s21_truncate) {
//    s21_decimal a = {{314, 0, 0, (2 << 16)}};
//    s21_decimal result = {{0, 0, 0, 0}};

//    int res = s21_truncate(a, &result);

//    ck_assert_int_eq(res, 0);
//    ck_assert_int_eq(result.bits[0], 3);
// }
// END_TEST

START_TEST(test_s21_negate) {
   s21_decimal a = {{314, 0, 0, (3 << 16)}};
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_negate(a, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 314);
   ck_assert_int_eq((uint32_t) result.bits[3] & (1 << 31), (uint32_t) (1 << 31));
}
END_TEST

START_TEST(test_s21_negate_positive) {
   s21_decimal value = {{123456, 0, 0, 0}}; // 123456
   s21_decimal expected = {{123456, 0, 0, 0}};
   s21_set_sign(&expected, 1);

   s21_decimal result;
   int res_code = s21_negate(value, &result);

   ck_assert_int_eq(res_code, 0);
   ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(test_s21_negate_negative) {
   s21_decimal value = {{123456, 0, 0, 0}};
   s21_set_sign(&value, 1); // -123456

   s21_decimal expected = {{123456, 0, 0, 0}}; // 123456

   s21_decimal result;
   int res_code = s21_negate(value, &result);

   ck_assert_int_eq(res_code, 0);
   ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(test_s21_negate_max_value) {
   s21_decimal value = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}}; // Максимальное число
   s21_decimal expected = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
   s21_set_sign(&expected, 1);

   s21_decimal result;
   int res_code = s21_negate(value, &result);

   ck_assert_int_eq(res_code, 0);
   ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(test_s21_negate_min_value) {
   s21_decimal value = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
   s21_set_sign(&value, 1); // -Максимальное число

   s21_decimal expected = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};

   s21_decimal result;
   int res_code = s21_negate(value, &result);

   ck_assert_int_eq(res_code, 0);
   ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(test_s21_negate_fractional) {
   s21_decimal value = {{123456, 0, 0, 0}};
   s21_set_scale(&value, 3); // 123.456

   s21_decimal expected = {{123456, 0, 0, 0}};
   s21_set_scale(&expected, 3);
   s21_set_sign(&expected, 1); // -123.456

   s21_decimal result;
   int res_code = s21_negate(value, &result);

   ck_assert_int_eq(res_code, 0);
   ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(test_s21_negate_invalid_decimal) {
   s21_decimal value = {{0, 0, 0, 0xFFFFFFFF}}; // Некорректный decimal
   s21_decimal result;
   
   int res_code = s21_negate(value, &result);

   ck_assert_int_eq(res_code, 1); // Должно вернуть ошибку
}
END_TEST

START_TEST(test_s21_from_int_to_decimal) {
   int a = 123456789;
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_from_int_to_decimal(a, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 123456789);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_negative) {
   int a = -987654321;
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_from_int_to_decimal(a, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 987654321); // По модулю
   ck_assert_int_eq(s21_get_sign(result), 1);  // Должно быть отрицательное
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_zero) {
   int a = 0;
   s21_decimal result = {{1, 1, 1, 1}}; // Задаем нестандартное значение

   int res = s21_from_int_to_decimal(a, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 0);
   ck_assert_int_eq(result.bits[1], 0);
   ck_assert_int_eq(result.bits[2], 0);
   ck_assert_int_eq(result.bits[3], 0); // Все биты должны быть 0
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_min_int) {
   int a = -2147483648; // Минимальное 32-битное число
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_from_int_to_decimal(a, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 2147483648); // Модуль числа
   ck_assert_int_eq(s21_get_sign(result), 1);  // Должно быть отрицательное
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_max_int) {
   int a = 2147483647; // Максимальное 32-битное число
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_from_int_to_decimal(a, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 2147483647);
   ck_assert_int_eq(s21_get_sign(result), 0);  // Должно быть положительное
}
END_TEST

START_TEST(test_s21_from_float_to_decimal) {
   float a = 123.456f;
   s21_decimal result = {{0, 0, 0, 0}};

   int res = s21_from_float_to_decimal(a, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result.bits[0], 123456);
   ck_assert_int_eq(result.bits[3] >> 16, 3);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int) {
   s21_decimal a = {{123456789, 0, 0, 0}};
   int result = 0;

   int res = s21_from_decimal_to_int(a, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_int_eq(result, 123456789);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float) {
   s21_decimal a = {{123456789, 0, 0, 0}};
   float result = 0.0f;

   int res = s21_from_decimal_to_float(a, &result);

   ck_assert_int_eq(res, 0);
   ck_assert_float_eq(result, 123456789.0f);
}
END_TEST

START_TEST(test_s21_mul_complex_fractional) {
   s21_decimal a = {{12345678, 0, 0, 0}};  // 1.2345678
   s21_decimal b = {{9876543, 0, 0, 0}};   // 0.9876543

   s21_set_scale(&a, 7);
   s21_set_scale(&b, 7);

   s21_decimal result;
   int res_code = s21_mul(a, b, &result);

   s21_decimal expected = {{12193262, 0, 0, 0}};  // 1.2193262
   s21_set_scale(&expected, 7);

   ck_assert_int_eq(res_code, 0);
   ck_assert_int_eq(result.bits[0], expected.bits[0]);
   ck_assert_int_eq(result.bits[1], expected.bits[1]);
   ck_assert_int_eq(result.bits[2], expected.bits[2]);
   ck_assert_int_eq(result.bits[3], expected.bits[3]);
}
END_TEST

//

START_TEST(test_s21_mul_bank_1) {
   // a * b = (2^97 – 5) / 2 = 2^97 - 2.5 = 79228162514264337593543950334.5 = 79228162514264337593543950334
   s21_decimal a = {{0xAAAAAAA9, 0xAAAAAAAA, 0xAAAAAAAA, 0}}; // (2^97 – 5) / 3
   s21_decimal b = {{15, 0, 0, 0}}; // 3 / 2

   s21_set_scale(&b, 1);

   s21_decimal result;
   int res_code = s21_mul(a, b, &result);

   s21_decimal expected = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};

   ck_assert_int_eq(res_code, 0);
   ck_assert_int_eq(result.bits[0], expected.bits[0]);
   ck_assert_int_eq(result.bits[1], expected.bits[1]);
   ck_assert_int_eq(result.bits[2], expected.bits[2]);
   ck_assert_int_eq(result.bits[3], expected.bits[3]);
}

START_TEST(test_s21_mul_bank_2) {
   s21_decimal a = {{0x66666665, 0x66666666, 0x66666666, 0}};
   s21_decimal b = {{25, 0, 0, 0}};
   s21_set_scale(&b, 1);

   s21_decimal result;
   int res_code = s21_mul(a, b, &result);

   s21_decimal expected = {{0xFFFFFFFC, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
   
   ck_assert_int_eq(res_code, 0);
   ck_assert_int_eq(result.bits[0], expected.bits[0]);
   ck_assert_int_eq(result.bits[1], expected.bits[1]);
   ck_assert_int_eq(result.bits[2], expected.bits[2]);
   ck_assert_int_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_s21_mul_bank_3) {
   s21_decimal a = {{0x5bfffffd, 0xd9d82b70, 0xcf8189f7, 0}}; // LSB изменён: 0xAAAAAAA8 (чётное)
   s21_decimal b = {{12345, 0, 0, 0}};  // 1.5
   s21_set_scale(&b, 4);

   s21_decimal result;
   int res_code = s21_mul(a, b, &result);

   // Ожидаемый результат – тот же X.5 округляется вниз, то есть остаётся равным X.
   // Например, если до округления было 2^96 - 3.5, то ожидаем 2^96 - 4.
   s21_decimal expected = {{0xFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
   
   ck_assert_int_eq(res_code, 0);
   ck_assert_int_eq(result.bits[0], expected.bits[0]);
   ck_assert_int_eq(result.bits[1], expected.bits[1]);
   ck_assert_int_eq(result.bits[2], expected.bits[2]);
   ck_assert_int_eq(result.bits[3], expected.bits[3]);
}
END_TEST

//


Suite *s21_decimal_suite(void) {
   Suite *s = suite_create("s21_decimal");
   TCase *tc_core = tcase_create("Core");

   tcase_add_test(tc_core, test_s21_mul_bank_1);
   tcase_add_test(tc_core, test_s21_mul_bank_2);
   tcase_add_test(tc_core, test_s21_mul_bank_3);

   //tcase_add_test(tc_core, test_s21_trim_trailing_zeros_basic);
   
   tcase_add_test(tc_core, test_s21_mul_complex_fractional);

   tcase_add_test(tc_core, test_s21_add_basic);
   tcase_add_test(tc_core, test_s21_add_positive);
   tcase_add_test(tc_core, test_s21_add_negative);
   tcase_add_test(tc_core, test_s21_add_opposite_numbers);
   tcase_add_test(tc_core, test_s21_add_overflow);
   tcase_add_test(tc_core, test_s21_add_with_zero);
   tcase_add_test(tc_core, test_s21_add_negative_overflow);
   tcase_add_test(tc_core, test_s21_add_complex_case);

   tcase_add_test(tc_core, test_s21_sub_basic);
   tcase_add_test(tc_core, test_s21_sub_underflow);
   tcase_add_test(tc_core, test_s21_sub_negative_result);
   tcase_add_test(tc_core, test_s21_sub_large_numbers);
   tcase_add_test(tc_core, test_s21_sub_same_numbers);
   tcase_add_test(tc_core, test_s21_sub_negative_zero);
   tcase_add_test(tc_core, test_s21_sub_overflow_negative);
   tcase_add_test(tc_core, test_s21_sub_carry_propagation);

   tcase_add_test(tc_core, test_s21_mul_basic);
   tcase_add_test(tc_core, test_s21_mul_overflow);
   tcase_add_test(tc_core, test_s21_mul_negative_result);
   tcase_add_test(tc_core, test_s21_mul_large_numbers);
   tcase_add_test(tc_core, test_s21_mul_zero);
   tcase_add_test(tc_core, test_s21_mul_negative_negative);
   tcase_add_test(tc_core, test_s21_mul_scale_handling);
   tcase_add_test(tc_core, test_s21_mul_carry_propagation);

   tcase_add_test(tc_core, test_s21_div_basic);
   tcase_add_test(tc_core, test_s21_div_by_zero);
   tcase_add_test(tc_core, test_s21_div_negative_result);

   tcase_add_test(tc_core, test_s21_is_less_basic);
   tcase_add_test(tc_core, test_s21_is_less_equal_numbers);
   tcase_add_test(tc_core, test_s21_is_less_negative_numbers);
   tcase_add_test(tc_core, test_s21_is_less_positive_negative);
   tcase_add_test(tc_core, test_s21_is_less_negative_positive);
   tcase_add_test(tc_core, test_s21_is_less_large_numbers);
   tcase_add_test(tc_core, test_s21_is_less_min_max);
   tcase_add_test(tc_core, test_s21_is_less_zero);
   tcase_add_test(tc_core, test_s21_is_less_zero_negative);

   tcase_add_test(tc_core, test_s21_is_less_equal);
   tcase_add_test(tc_core, test_s21_is_less_or_equal_negative_numbers);
   tcase_add_test(tc_core, test_s21_is_less_or_equal_positive_negative);

   tcase_add_test(tc_core, test_s21_is_greater);
   tcase_add_test(tc_core, test_s21_is_greater_equal_numbers);
   tcase_add_test(tc_core, test_s21_is_greater_negative_vs_positive);
   tcase_add_test(tc_core, test_s21_is_greater_positive_vs_negative);
   tcase_add_test(tc_core, test_s21_is_greater_negative_numbers);
   tcase_add_test(tc_core, test_s21_is_greater_large_numbers);
   tcase_add_test(tc_core, test_s21_is_greater_scale_difference);

   tcase_add_test(tc_core, test_s21_is_greater_or_equal);
   tcase_add_test(tc_core, test_s21_is_greater_or_equal_equal_numbers);
   tcase_add_test(tc_core, test_s21_is_greater_or_equal_negative_numbers);

   tcase_add_test(tc_core, test_s21_is_equal);
   tcase_add_test(tc_core, test_s21_is_equal_different_numbers);
   tcase_add_test(tc_core, test_s21_is_equal_negative_numbers);
   tcase_add_test(tc_core, test_s21_is_equal_positive_negative);
   tcase_add_test(tc_core, test_s21_is_equal_large_numbers);
   tcase_add_test(tc_core, test_s21_is_equal_zero);


   tcase_add_test(tc_core, test_s21_is_not_equal);

//    tcase_add_test(tc_core, test_s21_floor);
//    tcase_add_test(tc_core, test_s21_round);
//    tcase_add_test(tc_core, test_s21_truncate);

   tcase_add_test(tc_core, test_s21_negate);
   tcase_add_test(tc_core, test_s21_negate_positive);
   tcase_add_test(tc_core, test_s21_negate_negative);
   tcase_add_test(tc_core, test_s21_negate_max_value);
   tcase_add_test(tc_core, test_s21_negate_min_value);
   tcase_add_test(tc_core, test_s21_negate_fractional);
   tcase_add_test(tc_core, test_s21_negate_invalid_decimal);

   tcase_add_test(tc_core, test_s21_from_int_to_decimal);
   tcase_add_test(tc_core, test_s21_from_int_to_decimal_negative);
   tcase_add_test(tc_core, test_s21_from_int_to_decimal_zero);
   tcase_add_test(tc_core, test_s21_from_int_to_decimal_min_int);
   tcase_add_test(tc_core, test_s21_from_int_to_decimal_max_int);

   tcase_add_test(tc_core, test_s21_from_float_to_decimal);
   tcase_add_test(tc_core, test_s21_from_decimal_to_int);
   tcase_add_test(tc_core, test_s21_from_decimal_to_float);

   suite_add_tcase(s, tc_core);
   return s;
}

int main(void) {
   int number_failed;
   Suite *s = s21_decimal_suite();
   SRunner *sr = srunner_create(s);

   srunner_run_all(sr, CK_NORMAL);
   number_failed = srunner_ntests_failed(sr);
   srunner_free(sr);

   return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

