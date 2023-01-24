#include "common.h"

int sum_ints(int* number_1, int* number_2, int* result, int count_int) {
  bool transfer_bit = false;
  for (int i = 0; i < count_int; i++) {
    transfer_bit =
        sum_int(number_1[i], number_2[i], &(result[i]), transfer_bit);
  }
  if (transfer_bit) {
    set_bit_int(&(result[count_int]), 0);
  }

  return transfer_bit;
}

int sum_int(int number_1, int number_2, int* result, bool transfer_bit) {
  for (int i = 0; i < BITS_IN_INT; i++) {
    bool b_1 = get_bit_int(number_1, i);
    bool b_2 = get_bit_int(number_2, i);
    if (b_1 && b_2 && transfer_bit) {
      set_bit_int(result, i);
    } else if ((b_1 && b_2) || (b_1 && transfer_bit) || (b_2 && transfer_bit)) {
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

int div_decimal_with_remainder(s21_decimal dividend, s21_decimal divisor,
                               s21_decimal* quotient, s21_decimal* remainder) {
  int status = 0;
  if (!is_zero_decimal(divisor)) {
    s21_decimal complement = {
        0,
    };
    copy_decimal(divisor, &complement);
    clear_decimal(quotient);
    clear_decimal(remainder);
    convert_ints_to_twos_complement(complement.bits, INTS_IN_DECIMAL);
    for (int num_int = INTS_IN_DECIMAL - 1; num_int >= 0; num_int--) {
      for (int num_bit = BITS_IN_INT - 1; num_bit >= 0; num_bit--) {
        left_shift_decimal(remainder);
        if (get_bit_decimal(dividend, num_int, num_bit)) {
          set_bit_decimal(remainder, 0, 0);
        }
        if (is_greater_ints(remainder->bits, divisor.bits, INTS_IN_DECIMAL)) {
          sum_ints(remainder->bits, complement.bits, remainder->bits,
                   INTS_IN_DECIMAL);
          set_bit_decimal(quotient, num_int, num_bit);
        } else if (is_equal_ints(remainder->bits, divisor.bits,
                                 INTS_IN_DECIMAL)) {
          clear_decimal(remainder);
          set_bit_decimal(quotient, num_int, num_bit);
        } else {
          reset_bit_decimal(quotient, num_int, num_bit);
        }
      }
    }
  } else {
    status = 3;
  }
  return status;
}

int div_double_decimal_with_remainder(s21_double_decimal dividend,
                                      s21_double_decimal divisor,
                                      s21_double_decimal* quotient,
                                      s21_double_decimal* remainder) {
  int status = 0;
  if (!is_zero_double_decimal(divisor)) {
    s21_double_decimal complement = {
        0,
    };
    copy_double_decimal(divisor, &complement);
    clear_double_decimal(quotient);
    clear_double_decimal(remainder);
    convert_ints_to_twos_complement(complement.bits, 2 * INTS_IN_DECIMAL);
    for (int num_int = 2 * INTS_IN_DECIMAL - 1; num_int >= 0; num_int--) {
      for (int num_bit = BITS_IN_INT - 1; num_bit >= 0; num_bit--) {
        left_shift_double_decimal(remainder);
        if (get_bit_double_decimal(dividend, num_int, num_bit)) {
          set_bit_double_decimal(remainder, 0, 0);
        }
        if (is_greater_ints(remainder->bits, divisor.bits,
                            2 * INTS_IN_DECIMAL)) {
          sum_ints(remainder->bits, complement.bits, remainder->bits,
                   2 * INTS_IN_DECIMAL);
          set_bit_double_decimal(quotient, num_int, num_bit);
        } else if (is_equal_ints(remainder->bits, divisor.bits,
                                 2 * INTS_IN_DECIMAL)) {
          clear_double_decimal(remainder);
          set_bit_double_decimal(quotient, num_int, num_bit);
        } else {
          reset_bit_double_decimal(quotient, num_int, num_bit);
        }
      }
    }
  } else {
    status = 3;
  }
  return status;
}

void mul_double_decimal(s21_double_decimal value_1, s21_double_decimal value_2, s21_double_decimal* result) {
  clear_double_decimal(result);
  mul_ints(value_1.bits, value_2.bits, result->bits, 2 * INTS_IN_DECIMAL);
  set_sign_double_decimal(result, get_sign_double_decimal(value_1) ^ get_sign_double_decimal(value_2));
  set_exp_double_decimal(result, get_exp_double_decimal(value_1) + get_exp_double_decimal(value_2));
}

int add_decimal(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  s21_double_decimal double_v1 = {0,};
  s21_double_decimal double_v2 = {0,};
  s21_double_decimal double_r = {0,};
  decimal_to_double_decimal(value_1, &double_v1);
  decimal_to_double_decimal(value_2, &double_v2);
  normalization_double_decimal(&double_v1, &double_v2);
  sum_ints(double_v1.bits, double_v2.bits, double_r.bits, 2 * INTS_IN_DECIMAL);
  double_r.bits[2 * INTS_IN_DECIMAL] = double_v1.bits[2 * INTS_IN_DECIMAL];
  return double_decimal_to_decimal(double_r, result);
}

int sub_decimal(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  s21_double_decimal double_v1 = {0,};
  s21_double_decimal double_v2 = {0,};
  s21_double_decimal double_r = {0,};
  decimal_to_double_decimal(value_1, &double_v1);
  decimal_to_double_decimal(value_2, &double_v2);
  normalization_double_decimal(&double_v1, &double_v2);
  convert_double_decimal_to_twos_complement(&double_v2);
  sum_ints(double_v1.bits, double_v2.bits, double_r.bits, 2 * INTS_IN_DECIMAL);
  double_r.bits[2 * INTS_IN_DECIMAL] = double_v1.bits[2 * INTS_IN_DECIMAL];
  return double_decimal_to_decimal(double_r, result);
}

int is_greater_ints(int* value_1, int* value_2, int count_int) {
  bool less = false;
  for (int i = count_int - 1; i >= 0; i--) {
    if ((unsigned)(value_1[i]) < (unsigned)(value_2[i])) {
      less = false;
      break;
    } else if ((unsigned)(value_1[i]) > (unsigned)(value_2[i])) {
      less = true;
      break;
    }
  }
  return less;
}

int is_equal_ints(int* value_1, int* value_2, int count_int) {
  bool equal = true;
  for (int i = 0; i < count_int; i++) {
    if (value_1[i] != value_2[i]) {
      equal = false;
      break;
    }
  }
  return equal;
}

int div_double_decimal(s21_double_decimal dividend, s21_double_decimal divisor,
                       s21_double_decimal* result) {
  int status = 0;
  s21_double_decimal complement = {0,};
  copy_double_decimal(divisor, &complement);
  if (!is_zero_double_decimal(divisor)) {
    convert_ints_to_twos_complement(complement.bits, 2 * INTS_IN_DECIMAL);
    s21_double_decimal tmp = {
        0,
    };
    for (int num_int = 2 * INTS_IN_DECIMAL - 1; num_int >= 0; num_int--) {
      for (int num_bit = BITS_IN_INT - 1; num_bit >= 0; num_bit--) {
        left_shift_double_decimal(&tmp);
        if (get_bit_double_decimal(dividend, num_int, num_bit)) {
          set_bit_double_decimal(&tmp, 0, 0);
        }
        if (is_greater_ints(tmp.bits, divisor.bits, 2 * INTS_IN_DECIMAL)) {
          sum_ints(tmp.bits, complement.bits, tmp.bits, 2 * INTS_IN_DECIMAL);
          set_bit_double_decimal(result, num_int, num_bit);
        } else if (is_equal_ints(tmp.bits, divisor.bits, 2 * INTS_IN_DECIMAL)) {
          clear_double_decimal(&tmp);
          set_bit_double_decimal(result, num_int, num_bit);
        } else {
          reset_bit_double_decimal(result, num_int, num_bit);
        }
      }
    }
  } else {
    status = 3;
  }
  return status;
}

int mul_ints(int* value_1, int* value_2, int* result, int count_int) {
  int status = 0;
  int state = 0;
  for (int i = 0; i < count_int; i++) {
    for (int j = 0; j < BITS_IN_INT; j++) {
      if (get_bit_ints(value_2, i, j)) {
        if (state) {
          status |= 1;
        }
        status |= sum_ints(result, value_1, result, count_int);
        if (status) {
          break;
        }
      }
      if (get_bit_ints(value_1, count_int - 1, BITS_IN_INT - 1)) {
        state = 1;
      }
      left_shift_ints(value_1, count_int);
    }
    if (status) {
      break;
    }
  }
  return status;
}

void casting_exp_double_decimal(s21_double_decimal value, s21_double_decimal* result, int exp_new) {
  int exp = get_exp_double_decimal(value);
  int sign = get_sign_double_decimal(value);
  s21_double_decimal ten = {{10,}};
  s21_double_decimal tmp = {0,};
  value.bits[2 * INTS_IN_DECIMAL] = 0;
  for (; exp < exp_new; exp++) {
    clear_double_decimal(&tmp);
    int status = mul_ints(value.bits, ten.bits, tmp.bits, 2 * INTS_IN_DECIMAL);
    if (status) {
      break;
    }
    copy_double_decimal(tmp, &value);
    copy_double_decimal(tmp, result);
  }
  set_exp_double_decimal(result, exp);
  set_sign_double_decimal(result, sign);
}
