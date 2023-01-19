#include "common.h"

int copy_decimal(s21_decimal src, s21_decimal* dst) {
  return copy_ints(src.bits, dst->bits, INTS_IN_DECIMAL);
}

int copy_double_decimal(s21_double_decimal src, s21_double_decimal* dst) {
  return copy_ints(src.bits, dst->bits, 2 * INTS_IN_DECIMAL);
}

int copy_ints(int* src, int* dst, int count_int) {
  for (int i = 0; i <= count_int; i++) {
    dst[i] = src[i];
  }
  return true;
}
