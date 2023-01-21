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

void clear_full_decimal(s21_decimal* value) {
  for (int i = 0; i <= INTS_IN_DECIMAL; i++) {
    value->bits[i] = 0;
  }
}

int float_to_scientific_notation_base_10(float num, int len_mantissa,
                                         int* mantissa, int* exp) {
  int max_mantissa = pow(10, len_mantissa);
  *exp = 0;
  while (num >= max_mantissa) {
    num /= 10;
    *exp += 1;
  }
  while (num < max_mantissa) {
    num *= 10;
    *exp -= 1;
  }
  while (*exp < -EXP_MAX - 1 && *exp >= -EXP_MAX - len_mantissa) {
    num /= 10;
    *exp += 1;
  }
  int remainder = (int)num % 10;
  num /= 10;
  *exp += 1;
  if (remainder >= 5) {
    num += 1;
  }
  if (num >= max_mantissa) {
    num /= 10;
    *exp += 1;
  }
  *mantissa = (int)num;
  return true;
}
