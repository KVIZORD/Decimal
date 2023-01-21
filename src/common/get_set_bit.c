#include "common.h"

int get_bit_decimal(s21_decimal value, int num_int, int ind) {
  return get_bit_ints(value.bits, num_int, ind);
}

int set_bit_decimal(s21_decimal* value, int num_int, int ind) {
  return set_bit_ints(value->bits, num_int, ind);
}

int reset_bit_decimal(s21_decimal* value, int num_int, int ind) {
  return reset_bit_ints(value->bits, num_int, ind);
}

int get_bit_double_decimal(s21_double_decimal value, int num_int, int ind) {
  return get_bit_ints(value.bits, num_int, ind);
}

int set_bit_double_decimal(s21_double_decimal* value, int num_int, int ind) {
  return set_bit_ints(value->bits, num_int, ind);
}

int reset_bit_double_decimal(s21_double_decimal* value, int num_int, int ind) {
  return reset_bit_ints(value->bits, num_int, ind);
}

int get_bit_ints(int* value, int num_int, int ind) {
  return get_bit_int(value[num_int], ind);
}

int set_bit_ints(int* value, int num_int, int ind) {
  return set_bit_int(&(value[num_int]), ind);
}

int reset_bit_ints(int* value, int num_int, int ind) {
  return reset_bit_int(&(value[num_int]), ind);
}

int get_bit_int(int number, int ind) { return (number >> ind) & 1; }

int set_bit_int(int* number, int ind) {
  *number |= 1 << ind;
  return true;
}

int reset_bit_int(int* number, int ind) {
  *number &= ~(1 << ind);
  return true;
}
