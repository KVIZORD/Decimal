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


void change_exp_decimal(s21_decimal* value, int exp) {
    int exp_old = get_exp_decimal(*value);
    int exp_new = exp_old;
    s21_decimal tmp = {{0, 0, 0, 0}};
    for (int i = exp_old; i < exp; i++) {
        // bool status = s21_mul(*value, (s21_decimal){{10, 0, 0, 0}}, &tmp);
        bool status = 0;
        if (status) {
            break;
        }
        exp_new += 1;
        copy_decimal(tmp, value);
    }
    for (int i = exp; i < exp_old; i++) {
        // bool status = s21_div(*value, (s21_decimal){{10, 0, 0, 0}}, &tmp);
        bool status = 0;
        if (status) {
            break;
        }
        exp_new -= 1;
        copy_decimal(tmp, value);
    }
    set_exp_decimal(value, exp_new);
}
