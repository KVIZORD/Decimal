#include "s21_decimal.h"
#include <stdbool.h>


bool get_bit(int number, int ind) { 
    return number & (1 << ind);
}

void normalization(s21_decimal* value_1, int offset) {

}

void numbers_normalization(s21_decimal* value_1, s21_decimal* value_2) {
    if (value_1->exponent < value_2->exponent) {
        int offset = value_2->exponent - value_1->exponent;
        normalization(value_1, offset)
    } else if (value_1->exponent > value_2->exponent) {
        int offset = value_1->exponent - value_2->exponent;
        normalization(value_2, offset)
    }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    for (int i = 0; i < 3; i++) {
        result->bits[i] = value_1.bits[i] & value_2.bits[i];
        if (get_bit(value_1.bits[i], 31) && get_bit(value_2.bits[i], 31)) {

        }
    }


    return 0;
}

