#include "s21_decimal.h"
#include "common/common.h"


int main() {
    s21_decimal a = {{0, 0, 0,  6 << 16}};
    // s21_decimal b = {{0, 0, 0,  2 << 16}};
    s21_decimal c = {{0, 0, 0,  0}};

    a.bits[0] = 0xffffffff;
    // set_bit_decimal(&a, INTS_IN_DECIMAL, BITS_IN_INT - 1);
    // print_decimal(a);
    // a.bits[1] = 0xffffffff;
    // a.bits[2] = 0xffffffff;

    // b.bits[0] = 0xffffffff;
    // b.bits[1] = 0xffffffff;
    // b.bits[2] = 0xffffffff;

    // print_decimal_in_dec(a);
    // s21_round(a, &c);
    // s21_truncate(a, &c);
    s21_floor(a, &c);

    // printf("resultt = %d\n", s21_mul(a, b, &c));
    
    print_decimal_in_dec(a);
    // print_decimal_in_dec(b);
    print_decimal_in_dec(c);

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


// Сравнение отрицательных чисел
// <
int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
    normalization_decimal(&value_1, &value_2);
    return is_greater_ints(value_2.bits, value_1.bits, INTS_IN_DECIMAL);
}


// <=
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
    normalization_decimal(&value_1, &value_2);
    return is_greater_ints(value_2.bits, value_1.bits, INTS_IN_DECIMAL) || is_equal_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
}


// >
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
    normalization_decimal(&value_1, &value_2);
    return is_greater_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
}


// >=
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
    normalization_decimal(&value_1, &value_2);
    return is_greater_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL) || is_equal_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
}


// ==
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
    normalization_decimal(&value_1, &value_2);
    return is_equal_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
}


// !=
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
    normalization_decimal(&value_1, &value_2);
    return !is_equal_ints(value_1.bits, value_2.bits, INTS_IN_DECIMAL);
}


// ближайшее целое числа в сторону отрицательной бесконечности
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

