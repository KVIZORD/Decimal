#include "s21_decimal.h"
#include "common/common.h"
#include <math.h>

int main() {
    s21_decimal a = {{0, 0, 0,  3 << 16}};
    s21_decimal b = {{0, 0, 0,  0 << 16}};
    s21_decimal c = {{0, 0, 0,  0}};

    a.bits[0] = 0xffffffff;
    a.bits[1] = 0xffffffff;
    a.bits[2] = 0xffffffff;
    // set_sign_decimal(&a, 1);
    // float n;
    // printf("res = %d\n", s21_from_decimal_to_float(a, &n));
    // printf("n = %f\n", n);

    b.bits[0] = 5;
    // b.bits[0] = 0xffffffff;
    // b.bits[1] = 0xffffffff;
    // b.bits[2] = 0xffffffff;

    // print_decimal_in_dec(a);
    // s21_round(a, &c);
    // s21_truncate(a, &c);
    // s21_floor(a, &c);

    printf("result = %d\n", s21_mul(a, b, &c));
    
    print_decimal_in_dec(a);
    print_decimal_in_dec(b);
    print_decimal_in_dec(c);

    return 0;
}




int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    Status status = STATUS_OK;
    int mantissa = 0;
    int exp = 0;
    int sign = get_sign_float(src);
    clear_full_decimal(dst);
    set_sign_float(&src, sign);
    float_to_scientific_notation_base_10(src, FLOAT_NUMBER_SIGNIFICANT_DIGITS, &mantissa, &exp);
    copy_ints(&mantissa, dst->bits, 0);
    while (exp > EXP_MIN && status == STATUS_OK) {
        status = s21_mul(*dst, (s21_decimal){{10, 0, 0, 0}}, dst);
        exp -= 1;
    }
    if (status == STATUS_OK && -exp >= EXP_MIN && -exp <= EXP_MAX) {
        set_exp_decimal(dst, -exp);
        set_sign_decimal(dst, sign);
    } else {
        status = STATUS_ERR;
        clear_full_decimal(dst);
    }
    return status;
}


int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    print_decimal_in_dec(src);
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


int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    ArithmeticStatus status = OK;
    if (!normalization_decimal(&value_1, &value_2)) {
        int sign_1 = get_sign_decimal(value_1);
        int sign_2 = get_sign_decimal(value_2);
        int exp = get_exp_decimal(value_1);
        if (sign_1 == sign_2) {
            s21_double_decimal res = {0,};
            set_sign_decimal(result, sign_1);
            set_sign_double_decimal(&res, sign_1);
            set_exp_double_decimal(&res, exp);
            sum_ints(value_1.bits, value_2.bits, res.bits, INTS_IN_DECIMAL);
            print_double_decimal_in_dec(res);
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
            s21_double_decimal res = {0,};
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
    s21_double_decimal v_1 = {0,};
    s21_double_decimal v_2 = {0,};
    s21_double_decimal r_2 = {0,};
    decimal_to_double_decimal(value_1, &v_1);
    decimal_to_double_decimal(value_2, &v_2);

    // Умножение
    mul_ints(v_1.bits, v_2.bits, r_2.bits, 2 * INTS_IN_DECIMAL);
    set_sign_double_decimal(&r_2, get_sign_decimal(value_1) ^ get_sign_decimal(value_2));
    set_exp_double_decimal(&r_2, get_exp_decimal(value_1) + get_exp_decimal(value_2));

    // Обратное преобразование в DECIMAL
    return double_decimal_to_decimal(r_2, result);
}


int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    ArithmeticStatus status = OK;
    if (is_zero_decimal(value_2)) {
        status = DIV_ZERO;
    } else {
        s21_double_decimal v_1 = {0,};
        s21_double_decimal v_2 = {0,};
        s21_double_decimal r_2 = {0,};
        decimal_to_double_decimal(value_1, &v_1);
        decimal_to_double_decimal(value_2, &v_2);
        casting_exp_double_decimal(v_1, &v_1, EXP_MAX);
        div_double_decimal(v_1, v_2, &r_2);
        set_sign_double_decimal(&r_2, get_sign_decimal(value_1) ^ get_sign_decimal(value_2));
        set_exp_double_decimal(&r_2, get_exp_double_decimal(v_1) - get_exp_double_decimal(v_2));
        status = double_decimal_to_decimal(r_2, result);
    }    
    return status;
}


int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal quotient = {0,};
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
        int res = is_greater_ints(value_2.bits, value_1.bits, INTS_IN_DECIMAL);
        status = sign_1 ? !res : res;
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
    CompareStatus status = TRUE;
    normalization_decimal(&value_1, &value_2);
    int sign_1 = get_sign_decimal(value_1);
    int sign_2 = get_sign_decimal(value_2);
    if (sign_1 == sign_2) {
        int res = is_greater_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
        status = sign_1 ? res : !res;
    } else {
        status = sign_1 ? FALSE : TRUE;
    } 
    return status;
}


// >=
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
    CompareStatus status = TRUE;
    normalization_decimal(&value_1, &value_2);
    int sign_1 = get_sign_decimal(value_1);
    int sign_2 = get_sign_decimal(value_2);
    if (sign_1 == sign_2) {
        int res = is_greater_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
        status = sign_1 ? res : !res;
        status |= is_equal_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
    } else {
        status = FALSE;
    } 
    return status;
}


// ==
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
    CompareStatus status = FALSE;
    normalization_decimal(&value_1, &value_2);
    int sign_1 = get_sign_decimal(value_1);
    int sign_2 = get_sign_decimal(value_2);
    if (sign_1 == sign_2) {
        status = is_equal_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
    }
    return status;
}


// !=
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
    return !s21_is_equal(value_1, value_2);
}


// ближайшее целое число в сторону отрицательной бесконечности
int s21_floor(s21_decimal value, s21_decimal *result) {
    int exp = get_exp_decimal(value);
    s21_decimal remainder = {0,};
    s21_decimal then = {0,};
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
    s21_decimal remainder = {0,};
    s21_decimal then = {0,};
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


// цифры указанного Decimal числа; любые дробные цифры отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result) {
    int exp = get_exp_decimal(value);
    s21_decimal remainder = {0,};
    s21_decimal then = {0,};
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

