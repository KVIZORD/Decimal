#include "s21_decimal.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT_INT_IN_DECIMAL 3

static bool get_bit(int number, int ind);
static bool set_bit(int *number, int ind);
static bool reset_bit(int *number, int ind);
static bool sum_int(int value_1, int value_2, int *result, bool transfer_bit);
static void print_binary(s21_decimal value);

bool s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);


static bool get_bit(int number, int ind) { return number & (1 << ind); }

static bool set_bit(int *number, int ind) {
  *number |= 1 << ind;
  return true;
}
static bool reset_bit(int *number, int ind) {
  *number &= ~(1 << ind);
  return true;
}

static bool sum_int(int value_1, int value_2, int *result, bool transfer_bit) {
  for (int i = 0; i < 32; i++) {
    bool b_1 = get_bit(value_1, i);
    bool b_2 = get_bit(value_2, i);
    if (b_1 && b_2 && transfer_bit) {
      set_bit(result, i);
    } else if ((b_1 && b_2) || (b_1 && transfer_bit) || (b_2 && transfer_bit)) {
      reset_bit(result, i);
      transfer_bit = true;
    } else if (b_1 || b_2 || transfer_bit) {
      set_bit(result, i);
      transfer_bit = false;
    } else {
      reset_bit(result, i);
    }
  }
  return transfer_bit;
}

static void print_binary(s21_decimal value) {
  for (int i = 2; i >= 0; i--) {
    for (int j = 31; j >= 0; j--) {
      printf("%d", (value.bits[i] >> j) & 1);
    }
    printf("|");
  }
  printf(" Степень и знак: ");

  int scale_factor = (value.bits[3] >> 16) & 0xFF;
  char sign = (value.bits[3] >> 31) & 1 ? '-' : '+';
  printf("%d %c\n", scale_factor, sign);
}

bool s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  bool transfer_bit = false;
  for (int i = 0; i < COUNT_INT_IN_DECIMAL; i++) {
    transfer_bit = sum_int(value_1.bits[i], value_2.bits[i], &(result->bits[i]),
                           transfer_bit);
  }
  return transfer_bit;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  for (int i = 0; i < COUNT_INT_IN_DECIMAL; i++) {
    value_2.bits[i] = ~value_2.bits[i];
  }

  bool transfer_bit = s21_add(value_2, (s21_decimal){{1, 0, 0, 0}}, &value_2);

  transfer_bit = s21_add(value_1, value_2, result);
  return transfer_bit;
}


int main(){
  s21_decimal num1, num2, res;
  num1.bits[0] = 8;
  num1.bits[1] = 0;
  num1.bits[2] = 0;

  num2.bits[0] = 1;
  num2.bits[1] = 0;
  num2.bits[2] = 0;

  s21_sub(num1, num2, &res);
  print_binary(num1);
  print_binary(num2);
  print_binary(res);
  return 0;
}