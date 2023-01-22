#include "s21_decimal.h"

#include <limits.h>
#include <math.h>

#include "common/common.h"
//  1101101111100111000000000011010 10110001011000100101101110010101
//  00100010000000000000000000000000
//                                ||                              || |
// int main() {
//   // s21_decimal a = {{0, 0, 0, 0 << 16}};
//   // s21_decimal b = {{0, 0, 0, 1 << 16}};
//   // s21_decimal c = {{0, 0, 0, 0}};
//   // set_sign_decimal(&a, 1);
//   // set_sign_decimal(&b, 1);
//   // a.bits[0] = 0b00100010000000000000000000000000;
//   // a.bits[1] = 0b10110001011000100101101110010101;
//   // a.bits[2] = 0b1101101111100111000000000011010;
//   // set_sign_decimal(&a, 1);
//   // b.bits[0] = 1;
//   // b.bits[0] = 0xffffffff;
//   // b.bits[1] = 0xffffffff;
//   // b.bits[2] = 0xffffffff;

//   // s21_decimal val_1 = {{int_1, 0, 0, 1 << 31}};
//   // s21_decimal val_2 = {{int_2, 0, 0, 0}};

//   // printf("result = %d\n", s21_mul(a, b, &c));
//   // printf("result = %d\n", s21_is_less(a, b));

//   s21_decimal res = {
//       0,
//   };
//   printf("result = %d\n", s21_mul(val_1, val_2, &res));
//   print_decimal_in_dec(val_1);
//   print_decimal_in_dec(val_2);
//   print_decimal_in_dec(res);
//   // print_decimal_in_dec(a);
//   // print_decimal_in_dec(b);
//   // print_decimal_in_dec(c);
//   return 0;
// }

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  ArithmeticStatus status = OK;
  int sign_1 = get_sign_decimal(value_1);
  int sign_2 = get_sign_decimal(value_2);
  if (sign_1 == sign_2) {
    status = add_decimal(value_1, value_2, result);
  } else if (sign_1) {
    set_sign_decimal(&value_1, false);
    status = s21_sub(value_2, value_1, result);
  } else if (sign_2) {
    set_sign_decimal(&value_2, false);
    status = s21_sub(value_1, value_2, result);
  }
  return status;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  ArithmeticStatus status = OK;
  int sign_1 = get_sign_decimal(value_1);
  int sign_2 = get_sign_decimal(value_2);
  if (is_zero_decimal(value_1)) {  // если уменьшаемое равно нулю
    s21_negate(value_2, result);
  } else if (is_zero_decimal(value_2)) {  // если вычитаемое равно нулю
    copy_decimal(value_1, result);
  } else if (sign_1 == sign_2) {  // если знаки чисел одинаковы
    if (sign_1 && s21_is_greater(value_1, value_2)) {
      status = s21_sub(value_2, value_1, result);
      set_sign_decimal(result, !sign_1);
    } else if (!sign_1 && s21_is_less(value_1, value_2)) {
      status = s21_sub(value_2, value_1, result);
      set_sign_decimal(result, !sign_1);
    } else {
      status = sub_decimal(value_1, value_2, result);
    }
  } else if (sign_1) {  // если первое число отрицательное, второе -
                        // положительные
    set_sign_decimal(&value_2, true);
    status = s21_add(value_1, value_2, result);
  } else if (sign_2) {  // если первое число положительные, второе -
                        // отрицательное
    set_sign_decimal(&value_2, false);
    status = s21_add(value_1, value_2, result);
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
  clear_decimal(result);
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
  if (sign_1 && sign_2) {
    status = is_greater_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
  } else if (!sign_1 && !sign_2) {
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
  normalization_decimal(&value_1, &value_2);
  return s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2);
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

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  clear_full_decimal(dst);
  int status = STATUS_OK;
  if (dst) {
    if (src < 0) {
      set_sign_decimal(dst, 1);
      src *= -1;
    }
    dst->bits[0] = src;
  } else {
    status = STATUS_ERR;
  }

  return status;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  Status status = STATUS_OK;
  int mantissa = 0;
  int exp = 0;
  int sign = get_sign_float(src);
  src = fabs(src);
  clear_full_decimal(dst);
  if (src > 0 && src < pow(10, -EXP_MAX)) {
    status = STATUS_ERR;
  } else if (src != 0) {
    // приведение float
    float_to_scientific_notation_base_10(src, FLOAT_NUMBER_SIGNIFICANT_DIGITS,
                                         &mantissa, &exp);
    // printf("MANTISSA = %7d\n", mantissa);
    // printf("EXP = %7d\n", exp);
    copy_ints(&mantissa, dst->bits, 0);
    // print_decimal_in_dec(*dst);
    // сведение положительной экспоненты к нулю
    while (status == STATUS_OK && exp > EXP_MIN) {
      status = s21_mul(*dst, (s21_decimal){{10, 0, 0, 0}}, dst);
      exp -= 1;
    }
    // print_decimal_in_dec(*dst);
  }
  // если экспонента  MMMMMMM*10^() 1234567*10^28
  if (status == STATUS_OK && -exp >= EXP_MIN && -exp <= EXP_MAX) {
    set_exp_decimal(dst, -exp);
    set_sign_decimal(dst, sign);
  } else {
    status = STATUS_ERR;
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
  if (src.bits[1] != 0 || src.bits[2] != 0 || !dst) {
    status = STATUS_ERR;
  }
  if (!status) {
    *dst = get_sign_decimal(res) ? num * -1 : num;
  }
  return status;
}

// ближайшее целое число в сторону отрицательной бесконечности
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
  if (get_sign_decimal(value) && !is_zero_decimal(remainder)) {
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
  copy_decimal(value, result);
  return !set_sign_decimal(result, !get_sign_decimal(value));
}