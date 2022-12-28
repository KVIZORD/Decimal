#include "services.h"


void s21_copy_decimal(s21_decimal src, s21_decimal* dst) {
    for (int i = 0; i <= INTS_IN_DECIMAL; i++) {
        dst->bits[i] = src.bits[i];
    }
}

bool copy_arr_ints(int* src, int* dst, int count_int_src) {
    for (int i = 0; i < count_int_src; i++) {
        dst[i] = src[i];
    }
    return true;
}
