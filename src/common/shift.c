#include "common.h"

int left_shift_decimal(s21_decimal* value) {
  return left_shift_ints(value->bits, INTS_IN_DECIMAL);
}

int left_shift_double_decimal(s21_double_decimal* value) {
  return left_shift_ints(value->bits, 2 * INTS_IN_DECIMAL);
}

int left_shift_ints(int* ints, int num_int) {
  bool transfer_bit = get_bit_int(ints[num_int - 1], BITS_IN_INT - 1);
  for (int i = num_int - 1; i >= 0; i--) {
    ints[i] <<= 1;
    int bit = (i > 0) ? get_bit_int(ints[i - 1], BITS_IN_INT - 1) : 0;
    if (bit) {
      set_bit_int(&(ints[i]), 0);
    } else {
      reset_bit_int(&(ints[i]), 0);
    }
  }
  return transfer_bit;
}
