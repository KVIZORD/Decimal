#include "common.h"

void print_binary_int(int number) {
  for (int i = BITS_IN_INT - 1; i >= 0; i--) {
    printf("%d", ((unsigned int)number & (1 << i)) >> i);
  }
}

void print_binary_ints(int* ints, int count_int) {
  for (int i = count_int; i >= 0; i--) {
    print_binary_int(ints[i]);
    printf("|");
  }
  printf("\n");
}

void print_decimal(s21_decimal value) {
  for (int i = INTS_IN_DECIMAL; i >= 0; i--) {
    print_binary_int(value.bits[i]);
    printf("|");
  }
  printf("\n");
}

void print_double_decimal(s21_double_decimal value) {
  for (int i = 2 * INTS_IN_DECIMAL; i >= 0; i--) {
    print_binary_int(value.bits[i]);
    printf("|");
  }
  printf("\n");
}

void print_decimal_in_dec(s21_decimal value) {
  if (get_sign_decimal(value)) {
    printf("-");
  } else {
    printf("+");
  }
  char buf[32] = {
      0,
  };
  int i = 30;
  __int128_t val = decimal_to_int128(value);
  if (val == 0) {
    buf[i] = '0';
    i--;
  } else {
    for (; val > 0 && i > 0; i--, val /= 10) {
      buf[i] = "0123456789abcdef"[val % 10];
    }
  }
  for (; i > 0; i--) {
    buf[i] = ' ';
  }
  printf("%s", &(buf[i + 1]));
  printf("*10^(-%u)\n", get_exp_decimal(value));
}

void print_double_decimal_in_dec(s21_double_decimal value) {
  if (get_sign_double_decimal(value)) {
    printf("-");
  } else {
    printf("+");
  }
  char buf[45] = {0};
  int i = 43;
  __uint128_t val = double_decimal_to_int128(value);
  if (val == 0) {
    buf[i] = '0';
    i--;
  } else {
    for (; val > 0 && i > 0; i--, val /= 10) {
      buf[i] = "0123456789abcdef"[val % 10];
    }
  }
  for (; i > 0; i--) {
    buf[i] = ' ';
  }
  printf("%s", &(buf[i + 1]));
  printf("*10^(-%u)\n", get_exp_double_decimal(value));
}

__uint128_t decimal_to_int128(s21_decimal value) {
  __uint128_t cnt = 0;
  __uint128_t factor = 1;
  for (int i = 0; i < INTS_IN_DECIMAL; i++) {
    for (int j = 0; j < BITS_IN_INT; j++) {
      cnt += factor * (__uint128_t)get_bit_decimal(value, i, j);
      factor *= 2;
    }
  }
  return cnt;
}

__uint128_t double_decimal_to_int128(s21_double_decimal value) {
  __uint128_t cnt = 0;
  __uint128_t factor = 1;
  for (int i = 0; i < 2 * INTS_IN_DECIMAL; i++) {
    for (int j = 0; j < BITS_IN_INT; j++) {
      cnt += factor * (__uint128_t)get_bit_double_decimal(value, i, j);
      factor *= 2;
    }
  }
  return cnt;
}
