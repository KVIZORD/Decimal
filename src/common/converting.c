#include "common.h"


void convert_decimal_to_twos_complement(s21_decimal* value) {
  convert_decimal_to_ones_complement(value);
  s21_decimal one = {{1, 0, 0, 0}};
  set_sign_decimal(&one, get_sign_decimal(*value));
  set_exp_decimal(&one, get_exp_decimal(*value));
  s21_add(*value, one, value);
}

void convert_decimal_to_ones_complement(s21_decimal* value) {
  for (int i = 0; i < INTS_IN_DECIMAL; i++) {
    value->bits[i] = ~(value->bits[i]);
  }
}

void convert_double_decimal_to_twos_complement(s21_double_decimal* value) {
  convert_ints_to_twos_complement(value->bits, 2 * INTS_IN_DECIMAL);
}

void convert_double_decimal_to_ones_complement(s21_double_decimal* value) {
  convert_ints_to_ones_complement(value->bits, 2 * INTS_IN_DECIMAL);
}

void convert_ints_to_twos_complement(int* value, int count_int) {
  convert_ints_to_ones_complement(value, count_int);
  bool transfer_bit = sum_int(value[0], 1, &(value[0]), false);
  for (int i = 1; i < count_int; i++) {
    transfer_bit = sum_int(value[i], 0, &(value[i]), transfer_bit);
  }
}

void convert_ints_to_ones_complement(int* value, int count_int) {
  for (int i = 0; i < count_int; i++) {
    value[i] = ~(value[i]);
  }
}

int double_decimal_to_decimal(s21_double_decimal src, s21_decimal* dst) {
  // print_double_decimal_in_dec(src);
  int sign = get_sign_double_decimal(src);
  int exp = get_exp_double_decimal(src);
  ArithmeticStatus status = OK;
  s21_double_decimal remainder = {0,};
  // деление "double_decimal" на 10 пока его мантиса не поместится в "decimal"
  while (exp < 2 * EXP_MAX && get_width_number_bits_non_blunk(src.bits, 2 * INTS_IN_DECIMAL) > INTS_IN_DECIMAL * BITS_IN_INT) {
    div_double_decimal_with_remainder(src, (s21_double_decimal){{10, 0,}}, &src, &remainder);
    exp -= 1;
  }
  // print_double_decimal_in_dec(src);
  if (exp > EXP_MAX || exp < 0) {
    status = sign ? INF_NEGAT : INF_POSIT;
  } else if (get_width_number_bits_non_blunk(src.bits, 2 * INTS_IN_DECIMAL) > INTS_IN_DECIMAL * BITS_IN_INT) {
    status = sign ? INF_NEGAT : INF_POSIT;
  } else {
    set_exp_double_decimal(&src, exp);
    // print_double_decimal_in_dec(src);
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
      dst->bits[i] = src.bits[i];
    }
    dst->bits[INTS_IN_DECIMAL] = src.bits[2 * INTS_IN_DECIMAL];
    // print_decimal_in_dec(*dst);
    status = bank_round_decimal(dst, remainder.bits[0]);
  }
  return status;
}

void decimal_to_double_decimal(s21_decimal src, s21_double_decimal* dst) {
  for (int i = 0; i < INTS_IN_DECIMAL; i++) {
    dst->bits[i] = src.bits[i];
  }
  dst->bits[2 * INTS_IN_DECIMAL] = src.bits[INTS_IN_DECIMAL];
}

int normalization_decimal(s21_decimal* value_1, s21_decimal* value_2) {
    Status status = STATUS_OK;
    int exp_1 = get_exp_decimal(*value_1);
    int exp_2 = get_exp_decimal(*value_2);
    if (status == STATUS_OK && exp_1 > exp_2) {
        change_exp(value_2, exp_1);
        exp_2 = get_exp_decimal(*value_2);
        change_exp(value_1, exp_2);
        exp_1 = get_exp_decimal(*value_1);
    }

    if (status == STATUS_OK && exp_1 < exp_2) {
        change_exp(value_1, exp_2);
        exp_1 = get_exp_decimal(*value_1);
        change_exp(value_2, exp_1);
        exp_2 = get_exp_decimal(*value_2);
    }
    if (exp_1 != exp_2) {
        status = STATUS_ERR;
    }

    return status;
}

int normalization_double_decimal(s21_double_decimal* value_1, s21_double_decimal* value_2) {
    Status status = STATUS_OK;
    int exp_1 = get_exp_double_decimal(*value_1);
    int exp_2 = get_exp_double_decimal(*value_2);
    for (; exp_1 > exp_2; exp_2++) {
      mul_double_decimal(*value_2, (s21_double_decimal){{10, 0, 0, 0, 0, 0, 1 << BIT_EXP_START}}, value_2);
    }
    for (; exp_1 < exp_2; exp_1++) {
      mul_double_decimal(*value_1, (s21_double_decimal){{10, 0, 0, 0, 0, 0, 1 << BIT_EXP_START}}, value_1);
    }
    if (get_exp_double_decimal(*value_1) != get_exp_double_decimal(*value_2)) {
        status = STATUS_ERR;
    }
    return status;
}

void change_exp(s21_decimal* value, int exp) {
    s21_decimal tmp = {{0, 0, 0, 0}};
    int exp_old = get_exp_decimal(*value);
    int exp_new = exp_old;
    for (int i = exp_old; i < exp ; i++) {
        bool status = s21_mul(*value, (s21_decimal){{10, 0, 0, 0}}, &tmp);
        if (status || get_exp_decimal(tmp) != get_exp_decimal(*value)) {
            break;
        }
        copy_decimal(tmp, value);
        exp_new += 1;
    }
    for (int i = exp; i < exp_old; i++) {
        div_decimal_with_remainder(*value, (s21_decimal){{10, 0, 0, 0}}, value, &(s21_decimal){{0,}});
        exp_new -= 1;
    }
    set_exp_decimal(value, exp_new);
}

int bank_round_decimal(s21_decimal* dst, int remainder) {
  // print_decimal_in_dec(*dst);
  // printf("remainder = %d\n", remainder);
  ArithmeticStatus status = OK;
  bool is_even_last_number = false;
  if (remainder == 5) {
      s21_decimal last_number = {0,};
      s21_mod(*dst, (s21_decimal){{10, 0,}}, &last_number);
      if (last_number.bits[0] % 2) {
        is_even_last_number = true;
      }
  }
  if (remainder > 5 || (remainder == 5 && is_even_last_number)) {  
    s21_decimal one = {{1, 0,}};
    s21_decimal res = {0,};
    int transfer_bit = sum_ints(dst->bits, one.bits, res.bits, INTS_IN_DECIMAL);
    if (transfer_bit) {
      // status = INF_POSIT;
      status = get_sign_decimal(*dst) ? INF_NEGAT : INF_POSIT;
    } else {
      for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        dst->bits[i] = res.bits[i];
      }
    }
  } 
  return status;
}
