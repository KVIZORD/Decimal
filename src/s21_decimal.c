#include "s21_decimal.h"

#include "common/common.h"

// int main() {
//   s21_decimal dst = {{0, 0, 0, 0}};
//   int src = -__INT_MAX__;

//   s21_from_int_to_decimal(src, &dst);
//   print_decimal(dst);

//   s21_decimal src_2 = {{__UINT32_MAX__, __UINT32_MAX__, __UINT32_MAX__, 0}};
//   set_sign_decimal(&src_2, 1);
//   set_exp_decimal(&src_2, 28);
//   int dst_2 = 1;

//   print_decimal_in_dec(src_2);
//   s21_from_decimal_to_int(src_2, &dst_2);
//   printf("%d\n", dst_2);
//   return 0;
// }

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int status = STATUS_OK;
  if (dst) {
    if (src < 0) {
      set_sign_decimal(dst, 1);
      src *= -1;
    }
    dst->bits[0] = src;
    dst->bits[1] = 0;
    dst->bits[2] = 0;
  } else {
    status = STATUS_ERR;
  }

  return status;
}

int get_exp_float(float number) {
  int *num = (int *)(&number);
  reset_bit_int(num, BITS_IN_INT - 1);
  *num >>= 23;
  return *num - 127;
}

int get_sign_float(float number) {
  int num = (int)(number);
  return get_bit_int(num, BITS_IN_INT - 1);
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  // int* ptr = (int*)(&src);
  // 0 00000000 000 0000 0000 0100 0000 0000
  // 1,000 0000 0000 0100 0000 0000
  // 1 000 0000 000,0 0100 0000 0000
  // 1 + 2^(-13)
  print_decimal_in_dec(*dst);
  print_binary_int((*(int *)&src));
  printf("\n");

  printf("sign = %d\n", get_sign_float(src));
  printf("exp = %d\n", get_exp_float(src));

  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  s21_decimal res = src;
  s21_decimal ten = {{10, 0, 0, 0}};
  int status = STATUS_OK;
  int exp = get_exp_decimal(src);
  while (exp != 0) {
    div_decimal_with_remainder(res, ten, &res,
                               &(s21_decimal){
                                   0,
                               });
    exp -= 1;
  }
  int num = res.bits[0];
  if (res.bits[1] != 0 || res.bits[2] != 0 || !dst) {
    status = STATUS_ERR;
  }
  if (!status) {
    *dst = get_sign_decimal(res) ? num * -1 : num;
  }
  return status;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int exp = get_exp_decimal(src);
  int sign = get_sign_decimal(src);
  double numb = 0;
  for (int i = 0; i < INTS_IN_DECIMAL; i++) {
    for (int j = 0; j < BITS_IN_INT; j++) {
      if (get_bit_decimal(src, i, j)) {
        numb += pow(2, j + i * BITS_IN_INT);
      }
    }
  }
  for (int i = 0; i < exp; i++) {
    numb /= 10;
  }
  *dst = (float)numb;
  if (sign) {
    *dst *= -1;
  }
  return 0;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  ArithmeticStatus status = OK;
  if (!normalization_decimal(&value_1, &value_2)) {
    int sign_1 = get_sign_decimal(value_1);
    int sign_2 = get_sign_decimal(value_2);
    int exp = get_exp_decimal(value_1);
    if (sign_1 == sign_2) {
      s21_double_decimal res = {
          0,
      };
      set_sign_decimal(result, sign_1);
      set_sign_double_decimal(&res, sign_1);
      set_exp_double_decimal(&res, exp);
      sum_ints(value_1.bits, value_2.bits, res.bits, INTS_IN_DECIMAL);
      status = double_decimal_to_decimal(res, result);
    } else if (sign_1) {
      set_sign_decimal(&value_1, false);
      status = s21_sub(value_2, value_1, result);
    } else if (sign_2) {
      set_sign_decimal(&value_2, false);
      status = s21_sub(value_1, value_2, result);
    }
  }
  return status;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  ArithmeticStatus status = OK;
  if (!normalization_decimal(&value_1, &value_2)) {
    int sign_1 = get_sign_decimal(value_1);
    int sign_2 = get_sign_decimal(value_2);
    int exp = get_exp_decimal(value_1);
    if (sign_1 == sign_2) {
      s21_double_decimal res = {
          0,
      };
      set_sign_decimal(result, sign_1);
      set_sign_double_decimal(&res, sign_1);
      set_exp_double_decimal(&res, exp);
      convert_decimal_to_twos_complement(&value_2);
      sum_ints(value_1.bits, value_2.bits, res.bits, INTS_IN_DECIMAL);
      status = double_decimal_to_decimal(res, result);
    } else if (sign_1) {
      set_sign_decimal(&value_2, true);
      status = s21_add(value_1, value_2, result);
    } else if (sign_2) {
      set_sign_decimal(&value_2, false);
      status = s21_add(value_1, value_2, result);
    }
  }
  return status;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_double_decimal v_1 = {
      0,
  };
  s21_double_decimal v_2 = {
      0,
  };
  s21_double_decimal r_2 = {
      0,
  };
  decimal_to_double_decimal(value_1, &v_1);
  decimal_to_double_decimal(value_2, &v_2);

  // Умножение
  mul_ints(v_1.bits, v_2.bits, r_2.bits, 2 * INTS_IN_DECIMAL);
  set_sign_double_decimal(
      &r_2, get_sign_decimal(value_1) ^ get_sign_decimal(value_2));
  set_exp_double_decimal(&r_2,
                         get_exp_decimal(value_1) + get_exp_decimal(value_2));

  // Обратное преобразование в DECIMAL
  return double_decimal_to_decimal(r_2, result);
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  ArithmeticStatus status = OK;
  if (is_zero_decimal(value_2)) {
    status = DIV_ZERO;
  } else {
    s21_double_decimal v_1 = {
        0,
    };
    s21_double_decimal v_2 = {
        0,
    };
    s21_double_decimal r_2 = {
        0,
    };
    decimal_to_double_decimal(value_1, &v_1);
    decimal_to_double_decimal(value_2, &v_2);
    casting_exp_double_decimal(v_1, &v_1, EXP_MAX);
    div_double_decimal(v_1, v_2, &r_2);
    set_sign_double_decimal(
        &r_2, get_sign_decimal(value_1) ^ get_sign_decimal(value_2));
    set_exp_double_decimal(
        &r_2, get_exp_double_decimal(v_1) - get_exp_double_decimal(v_2));
    status = double_decimal_to_decimal(r_2, result);
  }
  return status;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal quotient = {
      0,
  };
  div_decimal_with_remainder(value_1, value_2, &quotient, result);
  return 1;
}

// <
int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  CompareStatus status = TRUE;
  normalization_decimal(&value_1, &value_2);
  int sign_1 = get_sign_decimal(value_1);
  int sign_2 = get_sign_decimal(value_2);
  if (sign_1 == sign_2) {
    status = is_greater_ints(value_2.bits, value_1.bits, INTS_IN_DECIMAL);
  } else if (is_zero_decimal(value_1) && is_zero_decimal(value_2)) {
    status = FALSE;
  } else {
    status = sign_1 ? TRUE : FALSE;
  }
  return status;
}

// <=
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  CompareStatus status = TRUE;
  normalization_decimal(&value_1, &value_2);
  int sign_1 = get_sign_decimal(value_1);
  int sign_2 = get_sign_decimal(value_2);
  if (sign_1 == sign_2) {
    int res = is_greater_ints(value_2.bits, value_1.bits, INTS_IN_DECIMAL);
    status = sign_1 ? !res : res;
    status |= is_equal_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
  } else {
    status = FALSE;
  }
  return status;
}

// >
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less_or_equal(value_1, value_2);
}

// >=
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less(value_1, value_2);
}

// ==
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  CompareStatus status = FALSE;
  normalization_decimal(&value_1, &value_2);
  int sign_1 = get_sign_decimal(value_1);
  int sign_2 = get_sign_decimal(value_2);
  if (sign_1 == sign_2) {
    status = is_equal_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
  } else if (is_zero_decimal(value_1) && is_zero_decimal(value_2)) {
    status = TRUE;
  }
  return status;
}

// !=
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}

// ближайшее целое числа в сторону отрицательной бесконечности
int s21_floor(s21_decimal value, s21_decimal *result) {
  int exp = get_exp_decimal(value);
  s21_decimal remainder = {
      0,
  };
  s21_decimal then = {
      0,
  };
  then.bits[0] = 10;
  for (; exp > 0; exp--) {
    div_decimal_with_remainder(value, then, &value, &remainder);
  }
  set_exp_decimal(&value, exp);

  if (get_sign_decimal(value)) {
    s21_decimal one = {{1, 0, 0, 0}};
    set_sign_decimal(&one, true);
    s21_add(value, one, &value);
  }

  copy_decimal(value, result);
  return 0;
}

// ближайшее целое число
int s21_round(s21_decimal value, s21_decimal *result) {
  int exp = get_exp_decimal(value);
  s21_decimal remainder = {
      0,
  };
  s21_decimal then = {
      0,
  };
  then.bits[0] = 10;
  for (; exp > 0; exp--) {
    div_decimal_with_remainder(value, then, &value, &remainder);
  }
  set_exp_decimal(&value, exp);

  s21_decimal five = {{5, 0, 0, 0}};
  if (s21_is_greater_or_equal(remainder, five)) {
    s21_decimal one = {{1, 0, 0, 0}};
    set_sign_decimal(&one, get_sign_decimal(value));
    s21_add(value, one, &value);
  }

  copy_decimal(value, result);
  return 0;
}

// цифры указанного Decimal числа; любые дробные цифры отбрасываются, включая
// конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int exp = get_exp_decimal(value);
  s21_decimal remainder = {
      0,
  };
  s21_decimal then = {
      0,
  };
  then.bits[0] = 10;
  for (; exp > 0; exp--) {
    div_decimal_with_remainder(value, then, &value, &remainder);
  }
  set_exp_decimal(&value, exp);
  copy_decimal(value, result);
  return 0;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  return !set_sign_decimal(result, !get_sign_decimal(value));
}