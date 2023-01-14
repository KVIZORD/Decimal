#include "common.h"


int get_exp(int number) {
    return (number << (BITS_IN_INT - 1 - BIT_EXP_END)) >> (BITS_IN_INT - 1 - BIT_EXP_END + BIT_EXP_START);
}

int set_exp(int* number, int exp) {
    int i = 0;
    int mask_clean_exp = ~(~((~i) << (BIT_EXP_END - BIT_EXP_START + 1)) << BIT_EXP_START);
    *number &= mask_clean_exp;
    *number |= exp << BIT_EXP_START;
    return true;
}

int get_exp_decimal(s21_decimal value) {
    return get_exp(value.bits[INTS_IN_DECIMAL]);
}

int set_exp_decimal(s21_decimal* value, int exp) {
    return set_exp(&(value->bits[INTS_IN_DECIMAL]), exp);
}

int get_exp_double_decimal(s21_double_decimal value) {
    return get_exp(value.bits[2 * INTS_IN_DECIMAL]);
}

int set_exp_double_decimal(s21_double_decimal* value, int exp) {
    return set_exp(&(value->bits[2 * INTS_IN_DECIMAL]), exp);
}

int get_sign(int number) {
    return get_bit_int(number, BIT_SIGN);
}

int set_sign(int* number, int sign) {
    int res = 0;
    if (sign) {
        res = set_bit_int(number, BIT_SIGN);
    } else {
        res = reset_bit_int(number, BIT_SIGN);
    }
    return res;
}

int get_sign_decimal(s21_decimal value) {
    return get_sign(value.bits[INTS_IN_DECIMAL]);
}

int set_sign_decimal(s21_decimal* value, int sign) {
    return set_sign(&(value->bits[INTS_IN_DECIMAL]), sign);
}

int get_sign_double_decimal(s21_double_decimal value) {
    return get_sign(value.bits[2 * INTS_IN_DECIMAL]);
}

int set_sign_double_decimal(s21_double_decimal* value, int sign) {
    return set_sign(&(value->bits[2 * INTS_IN_DECIMAL]), sign);
}

int get_sign_float(float number) {
    int num = (int)(number);
    return get_bit_int(num, BITS_IN_INT - 1);
}

int set_sign_float(float* number, int sign) {
    int* num = (int*)(number);
    int status = true;
    if (sign) {
        status = reset_bit_int(num, BITS_IN_INT - 1);
    }
    return status;
}
