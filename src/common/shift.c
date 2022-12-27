#include "services.h"


bool left_shift_decimal_ints(s21_decimal* value) {
    return left_shift_one_arr_int(value->bits, INTS_IN_DECIMAL);
}

bool left_shift_arr_int(int* ints, int num_int, unsigned int offset) {
    bool transfer_bit = false;
    for (unsigned int i = 0; i < offset; i++) {
       transfer_bit = left_shift_one_arr_int(ints, num_int) ? true : transfer_bit;
    }
    return transfer_bit;
}

bool left_shift_one_arr_int(int* ints, int num_int) {
    bool transfer_bit = get_bit_int(ints[num_int - 1], BITS_IN_INT - 1);
    for (int i = num_int - 1; i >= 0; i--) {
        ints[i] <<= 1;
        if (i > 0) {
            ints[i] |= ((ints[i - 1] >> (BITS_IN_INT - 1)) & 1);
        }
    }
    return transfer_bit;
}

void shift_point(s21_decimal* value, int exp) {
    s21_decimal tmp = {{0, 0, 0, 0}};
    int exp_old = get_exponent(*value);
    int exp_new = exp_old;
    for (int i = exp_old; i < exp; i++) {
        bool status = s21_mul(*value, (s21_decimal){{10, 0, 0, 0}}, &tmp);
        if (status) {
            break;
        }
        exp_new += 1;
        s21_copy_decimal(tmp, value);
    }
    for (int i = exp; i < exp_old; i++) {
        bool status = s21_div(*value, (s21_decimal){{10, 0, 0, 0}}, &tmp);
        s21_copy_decimal(tmp, value);
        if (status) {
            break;
        }
        exp_new -= 1;
        s21_copy_decimal(tmp, value);
    }
    set_exponent(value, exp_new);
}

bool right_shift_decimal(s21_decimal* value, bool left_bit) {
    bool transfer_bit = get_bit_int((value->bits)[0], 0);
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        value->bits[i] >>= 1;
        if (i < (INTS_IN_DECIMAL - 1)) {
            if (get_bit_decimal(*value, i + 1, 0)) {
                set_bit_decimal(value, i, BITS_IN_INT - 1);
            } else {
                reset_bit_decimal(value, i, BITS_IN_INT - 1);
            }
        }
    }
    if (left_bit) {
        set_bit_decimal(value, INTS_IN_DECIMAL - 1, BITS_IN_INT - 1);
    } else {
        reset_bit_decimal(value, INTS_IN_DECIMAL - 1, BITS_IN_INT - 1);
    }
    return transfer_bit;
}


