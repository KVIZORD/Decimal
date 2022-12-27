#include "services.h"


int get_sum_exponent(s21_decimal value_1, s21_decimal value_2) {
    int exp_1 = get_exponent(value_1);
    int exp_2 = get_exponent(value_2);
    int exp = exp_1 + exp_2;
    return exp;
}

int get_exponent(s21_decimal value) {
    return (value.bits[META_NUMBER_INT - 1] << (BITS_IN_INT - 1 - BIT_EXP_END)) >> (BITS_IN_INT - 1 - BIT_EXP_END + BIT_EXP_START);
}

void set_exponent(s21_decimal* value, int exp) {
    int i = 0;
    value->bits[META_NUMBER_INT - 1] &= ~((~i) << (BITS_IN_INT - 1 - BIT_EXP_END)) >> (BITS_IN_INT - 1 - BIT_EXP_END + BIT_EXP_START);
    value->bits[META_NUMBER_INT - 1] |= exp << BIT_EXP_START;
}

int get_sign(s21_decimal value) {
    return get_bit_decimal(value, INTS_IN_DECIMAL, BIT_SIGN - 1);
}

void set_sign(s21_decimal* value, int sign) {
    if (sign) {
        set_bit_decimal(value, INTS_IN_DECIMAL, BIT_SIGN);
    } else {
        reset_bit_decimal(value, INTS_IN_DECIMAL, BIT_SIGN);
    }
}
