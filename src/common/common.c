#include "common.h"


int get_width_number_bits_non_blunk(int* ints, int count_int) {
    int width = 0;
    for (int i = 0; i < count_int; i++) {
        for (int j = 0; j < BITS_IN_INT; j++) {
            if (get_bit_int(ints[i], j)) {
                width = i * BITS_IN_INT + j + 1;
            }
        }
    }
    return width;
}

int is_zero_decimal(s21_decimal value) {
    bool zero = true;
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        if (value.bits[i]) {
            zero = false;
            break;
        }
    }
    return zero;
}

int is_zero_double_decimal(s21_double_decimal value) {
    bool zero = true;
    for (int i = 0; i < 2 * INTS_IN_DECIMAL; i++) {
        if (value.bits[i]) {
            zero = false;
            break;
        }
    }
    return zero;
}

void clear_decimal(s21_decimal* value) {
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        value->bits[i] = 0;
    }
}

void clear_double_decimal(s21_double_decimal* value) {
    for (int i = 0; i < 2 * INTS_IN_DECIMAL; i++) {
        value->bits[i] = 0;
    }
}
