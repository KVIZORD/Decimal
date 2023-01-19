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
  int exp = get_exp_double_decimal(src);
  ArithmeticStatus status = OK;
  s21_double_decimal then = {
      0,
  };
  s21_double_decimal prev = {
      0,
  };
  then.bits[0] = 10;
  while (exp < 2 * EXP_MAX &&
         get_width_number_bits_non_blunk(src.bits, 2 * INTS_IN_DECIMAL) >
             INTS_IN_DECIMAL * BITS_IN_INT) {
    copy_double_decimal(src, &prev);
    div_double_decimal(src, then, &src);
    exp -= 1;
  }
  if (exp > EXP_MAX) {
    status = INF_NEGAT;
  } else if (exp < EXP_MIN) {
    status = INF_POSIT;
  } else if (get_width_number_bits_non_blunk(src.bits, 2 * INTS_IN_DECIMAL) >
             INTS_IN_DECIMAL * BITS_IN_INT) {
    status = INF_POSIT;
  } else {
    set_exp_double_decimal(&src, exp);
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
      dst->bits[i] = src.bits[i];
    }
    dst->bits[INTS_IN_DECIMAL] = src.bits[2 * INTS_IN_DECIMAL];
    s21_double_decimal remainder = {
        0,
    };
    div_double_decimal_with_remainder(
        prev, (s21_double_decimal){{10, 0, 0, 0, 0, 0, 0}},
        &(s21_double_decimal){
            0,
        },
        &remainder);
    bank_round_decimal(dst, remainder.bits[0]);
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

void change_exp(s21_decimal* value, int exp) {
  s21_decimal tmp = {{0, 0, 0, 0}};
  int exp_old = get_exp_decimal(*value);
  int exp_new = exp_old;
  for (int i = exp_old; i < exp; i++) {
    bool status = s21_mul(*value, (s21_decimal){{10, 0, 0, 0}}, &tmp);
    if (status || get_exp_decimal(tmp) != get_exp_decimal(*value)) {
      break;
    }
    copy_decimal(tmp, value);
    exp_new += 1;
  }
  for (int i = exp; i < exp_old; i++) {
    div_decimal_with_remainder(*value, (s21_decimal){{10, 0, 0, 0}}, value,
                               &(s21_decimal){{
                                   0,
                               }});
    exp_new -= 1;
  }
  set_exp_decimal(value, exp_new);
}

int bank_round_decimal(s21_decimal* dst, int remainder) {
  ArithmeticStatus status = OK;
  s21_double_decimal res = {
      0,
  };
  s21_decimal one = {{1, 0, 0, 0}};
  if (remainder == 5) {
    s21_decimal then = {{10, 0, 0, 0}};
    s21_decimal last_number = {
        0,
    };
    s21_mod(*dst, then, &last_number);
    if (last_number.bits[0] % 2) {
      sum_ints(dst->bits, one.bits, res.bits, INTS_IN_DECIMAL);
      status = double_decimal_to_decimal(res, dst);
    }
  } else if (remainder > 5) {
    sum_ints(dst->bits, one.bits, res.bits, INTS_IN_DECIMAL);
    status = double_decimal_to_decimal(res, dst);
  }
  return status;
}
