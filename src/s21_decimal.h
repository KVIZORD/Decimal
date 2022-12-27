#pragma once
#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_
// #include "common/bit_operations.h"
// #include "common/print.h"

#define BITS_IN_INT 32          // кол-во бит в инте
#define INTS_IN_DECIMAL 3       // кол-во INT хранящих значащее число
#define META_NUMBER_INT 4       // номер INT хранящий знак и порядок в decimal
#define BIT_SIGN BITS_IN_INT    // номер бита знака в INT
#define BIT_EXP_START 16        // номер бита с которого хранится экспонента
#define BIT_EXP_END 23          // номер юита до которого хранится экспонента
#define EXP_MAX 28              // максимальное значение экспоненты
#define EXP_MIN 0               // минимальное значение экспоненты

typedef struct {
    int bits[1 + INTS_IN_DECIMAL];
} s21_decimal;

typedef struct {
    int bits[1 + 2 * INTS_IN_DECIMAL];
} s21_double_decimal;

typedef enum ArithmeticStatus {
    OK,         // 0 - OK
    INF_POSIT,  // 1 - число слишком велико или равно бесконечности
    INF_NEGAT,  // 2 - число слишком мало или равно отрицательной бесконечности
    DIV_ZERO    // 3 - деление на 0
} ArithmeticStatus;

typedef enum CompareStatus {
    FALSE,      // 0 - FALSE
    TRUE        // 1 - TRUE
} CompareStatus;

typedef enum Status {
    STATUS_OK,          // 0 - OK
    STATUS_ERR          // 1 - ошибка
} Status;

// typedef enum CalcStatus {
//     OK,         // 0 - OK
//     ERR         // 1 - ошибка вычисления
// } CalcStatus;


int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);



#endif  // SRC_S21_DECIMAL_H_
