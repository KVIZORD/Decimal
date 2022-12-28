#include "services.h"


// DECIMAL

int get_sum_exponent(s21_decimal value_1, s21_decimal value_2) {
    int exp_1 = get_exponent_decimal(value_1);
    int exp_2 = get_exponent_decimal(value_2);
    int exp = exp_1 + exp_2;
    return exp;
}

int get_exponent_decimal(s21_decimal value) {
    return (value.bits[META_NUMBER_INT - 1] << (BITS_IN_INT - 1 - BIT_EXP_END)) >> (BITS_IN_INT - 1 - BIT_EXP_END + BIT_EXP_START);
}

void set_exponent_decimal(s21_decimal* value, int exp) {
    int i = 0;
    value->bits[META_NUMBER_INT - 1] &= ~((~i) << (BITS_IN_INT - 1 - BIT_EXP_END)) >> (BITS_IN_INT - 1 - BIT_EXP_END + BIT_EXP_START);
    value->bits[META_NUMBER_INT - 1] |= exp << BIT_EXP_START;
}

int get_sign_decimal(s21_decimal value) {
    return get_bit_decimal(value, INTS_IN_DECIMAL, BIT_SIGN - 1);
}

void set_sign_decimal(s21_decimal* value, int sign) {
    if (sign) {
        set_bit_decimal(value, INTS_IN_DECIMAL, BIT_SIGN);
    } else {
        reset_bit_decimal(value, INTS_IN_DECIMAL, BIT_SIGN);
    }
}


// DOUBLE_DECIMAL

int get_exponent_double_decimal(s21_double_decimal value) {
    return (value.bits[2 * INTS_IN_DECIMAL] << (BITS_IN_INT - 1 - BIT_EXP_END)) >> (BITS_IN_INT - 1 - BIT_EXP_END + BIT_EXP_START);
}

void set_exponent_double_decimal(s21_double_decimal* value, int exp) {
    int i = 0;
    value->bits[2 * INTS_IN_DECIMAL] &= ~((~i) << (BITS_IN_INT - 1 - BIT_EXP_END)) >> (BITS_IN_INT - 1 - BIT_EXP_END + BIT_EXP_START);
    value->bits[2 * INTS_IN_DECIMAL] |= exp << BIT_EXP_START;
}

int get_sign_double_decimal(s21_double_decimal value) {
    return get_bit_double_decimal(value, 2 * INTS_IN_DECIMAL, BIT_SIGN - 1);
}

void set_sign_double_decimal(s21_double_decimal* value, int sign) {
    if (sign) {
        set_bit_double_decimal(value, 2 * INTS_IN_DECIMAL, BIT_SIGN);
    } else {
        reset_bit_double_decimal(value, 2 * INTS_IN_DECIMAL, BIT_SIGN);
    }
}


