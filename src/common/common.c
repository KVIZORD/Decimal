#include "services.h"

int s21_div_with_remainder(s21_decimal dividend, s21_decimal divisor, s21_decimal *quotient, s21_decimal *remainder) {
    int status = 0;
    if (!s21_is_zero_decimal(divisor)) {
        s21_decimal tmp = {0,};
        for (int num_int = INTS_IN_DECIMAL - 1; num_int >= 0; num_int--) {
            for (int num_bit = BITS_IN_INT - 1; num_bit >= 0; num_bit--) {
                // сдвиг уменьшаемого числа влево на один разряд
                bool state_bit = get_bit_decimal(dividend, num_int, num_bit);
                left_shift_decimal_ints(&tmp);
                // перенос бита в нулевую позицию сдвинутого числа
                if (state_bit) {
                    set_bit_decimal(&tmp, 0, 0);
                }
                // вычисление остатка
                if (s21_is_greater(tmp, divisor)) {
                    // остататок > 0
                    s21_sub(tmp, divisor, &tmp);
                    set_bit_decimal(quotient, num_int, num_bit);
                } else if (s21_is_equal(tmp, divisor)) {
                    // остататок = 0
                    decimal_clear(&tmp);
                    set_bit_decimal(quotient, num_int, num_bit);
                }
            }
        }
        s21_copy_decimal(tmp, remainder);
    } else {
        status = 3;
    }
    return status;
}

void s21_copy_decimal(s21_decimal src, s21_decimal* dst) {
    for (int i = 0; i <= INTS_IN_DECIMAL; i++) {
        dst->bits[i] = src.bits[i];
    }
}

bool s21_is_zero_decimal(s21_decimal value) {
    bool zero = true;
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        if (value.bits[i]) {
            zero = false;
            break;
        }
    }
    return zero;
}

void decimal_clear(s21_decimal* value) {
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        value->bits[i] = 0;
    }
}

bool copy_arr_ints(int* src, int* dst, int count_int_src) {
    for (int i = 0; i < count_int_src; i++) {
        dst[i] = src[i];
    }
    return true;
}

int sub_arr_int(int* value_1, int* value_2, int* result, int count_int) {
  for (int i = 0; i < count_int; i++) {
    value_2[i] = ~(value_2[i]);
    result[i] = 0;
  }
  set_bit_arr_int(result, 0, 0);  
  bool transfer_bit = sum_arr_int(value_2, result, value_2, count_int);
  reset_bit_arr_int(result, 0, 0);
  transfer_bit = sum_arr_int(value_1, value_2, result, count_int);
  return transfer_bit;
}

bool sum_arr_int(int* value_1, int* value_2, int* result, int count_int) {
    bool transfer_bit = false;
    for (int i = 0; i < count_int; i++) {
        transfer_bit = sum_ints(value_1[i], value_2[i], &(result)[i], transfer_bit);
    }
    return transfer_bit;
}


// ++-
bool sum_ints(int value_1, int value_2, int* result, bool transfer_bit) {
    // побитовое сложение
    for (int i = 0; i < BITS_IN_INT; i++) {
        bool b_1 = get_bit_int(value_1, i);
        bool b_2 = get_bit_int(value_2, i);
        if (b_1 && b_2 && transfer_bit) {
            set_bit_int(result, i);
        } else if ((b_1 && b_2) || (b_1 && transfer_bit) || (b_2 &&transfer_bit)) {
            reset_bit_int(result, i);
            transfer_bit = true;
        } else if (b_1 || b_2 || transfer_bit) {
            set_bit_int(result, i);
            transfer_bit = false;
        } else {
            reset_bit_int(result, i);
        }
    }
    return transfer_bit;
}



// +++
int s21_sum(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    bool transfer_bit = false;
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        transfer_bit = sum_ints(value_1.bits[i], value_2.bits[i], &(result->bits[i]), transfer_bit);
    }
    int exp = get_exponent(value_1);
    set_sign(result, get_sign(value_1));
    set_exponent(result, exp);
    
    return transfer_bit;
}

