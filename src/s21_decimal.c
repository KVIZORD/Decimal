#include "s21_decimal.h"
#include "common/services.h"
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>



bool sum_ints(int value_1, int value_2, int* result, bool transfer_bit);
bool sum_arr_int(int* value_1, int* value_2, int* result, int count_int);
int sub_arr_int(int* value_1, int* value_2, int* result, int count_int);
int s21_div_with_remainder(s21_decimal dividend, s21_decimal divisor, s21_decimal *quotient, s21_decimal *remainder);
void s21_copy_decimal(s21_decimal src, s21_decimal* dst);
bool s21_is_zero_decimal(s21_decimal value);
bool copy_arr_ints(int* src, int* dst, int count_int_src);

void decimal_clear(s21_decimal* value);
void round_banking(s21_decimal* value, bool right_bit);

int s21_sum(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
// int get_sign_decimal(s21_decimal value);
// void set_sign_decimal(s21_decimal* value, int sign);
// int get_exponent_decimal(s21_decimal value);
// void set_exponent_decimal(s21_decimal* value, int exp);
// int get_sum_exponent(s21_decimal value_1, s21_decimal value_2);
void convert_decimal_to_ones_complement(s21_decimal* value);
void convert_decimal_to_twos_complement(s21_decimal* value);


int normalization_decimal(s21_decimal* value_1, s21_decimal* value_2);

// 1.3292279165568E+36
// +29710560942849126597578981375*10^(-3)
// +49517601571415210995964968959*10^(-3)
// +79228162514264337593543950334*10^(-3)
int main() {
// 79,228,162,514,264,337,593,543,950,335*10^(-3)
// 79,228,162,514,264,337,593,543,950,335
    s21_decimal a = {{0, 0, 0,  3 << 16}};
    s21_decimal b = {{0, 0, 0,  3 << 16}};
    s21_decimal c = {{0, 0, 0,  0}};
    a.bits[0] = 0xffffffff;
    a.bits[1] = 0xffffffff;
    a.bits[2] = 0xffffffff;
    // a.bits[2] = 0xfffffff;
    // b.bits[0] = 10;
    b.bits[0] = 0xffffff;
    // b.bits[1] = 0xffffffff;
    // b.bits[2] = 0xffffffff;
    // normalization_decimal(&a, &b);
    s21_mul(a, b, &c);
    // convert_decimal_to_twos_complement(&a);
    // printf("%u\n", right_shift_decimal(&a, 1));
    print_decimal(a);
    print_decimal(b);
    print_decimal(c);
    print_decimal_in_dec(a);
    print_decimal_in_dec(b);
    print_decimal_in_dec(c);
    // printf("%u\n", get_exponent_decimal(a));
    // printf("%u\n", get_exponent_decimal(b));
    // printf("%u\n", get_exponent_decimal(c));
    // printf("sign = %u\n", get_sign_decimal(a));
    // return 0;
    // s21_mul(a, b, &c);
    // s21_div(a, b, &c);
    // s21_mod(a, b, &c);
    // print_decimal(a);
    // print_decimal(b);
    // shift_point(&a, 29);
    // printf("%u\n", a.bits[0]);
    // printf("%u\n", get_exponent_decimal(a));
    // printf("%u\n", get_sign_decimal(c));
    // get_bit_decimal

    // printf("%u\n", a.bits[0] + b.bits[0]);

    return 0;
}


int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    ArithmeticStatus status = OK;
    Status status_ = normalization_decimal(&value_1, &value_2);
    if (status_ == STATUS_OK) {
        int sign_2 = get_sign_decimal(value_2);
        int sign_1 = get_sign_decimal(value_1);
        int exp = get_exponent_decimal(value_1);
        if (sign_1 == sign_2) {
            set_sign_decimal(result, sign_1);
            set_exponent_decimal(result, exp);
            bool transfer_bit = s21_sum(value_1, value_2, result);
            // если случилось переполнение и степень больше нуля
            if (transfer_bit && exp >= 1) {
                transfer_bit = false;
                bool right_bit = right_shift_decimal(result, true);    // сдвиг вправо - т.е. деление на 2
                round_banking(result, right_bit);
                s21_div(*result, (s21_decimal){{1, 0, 0, 0}}, result);      // деление на 5
                set_exponent_decimal(result, exp - 1);                              // уменьшение степени
            }
        } else if (sign_1) {
            status = s21_sub(value_2, value_1, result);
        } else if (sign_2) {
            status = s21_sub(value_1, value_2, result);
        }
    }
    return status;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    ArithmeticStatus status = OK;
    Status status_ = normalization_decimal(&value_1, &value_2);
    if (status_ == STATUS_OK) {
        int sign_2 = get_sign_decimal(value_2);
        int sign_1 = get_sign_decimal(value_1);
        int exp = get_exponent_decimal(value_1);
        if (sign_1 == sign_2) {
            set_sign_decimal(result, sign_1);
            set_exponent_decimal(result, exp);
            convert_decimal_to_twos_complement(&value_2);
            status = s21_sum(value_1, value_2, result);
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
    int status = 0;
    int exp_1 = get_exponent_decimal(value_1);
    int exp_2 = get_exponent_decimal(value_2);
    // printf("exp_1 + exp_2 = %d\n", exp_1 + exp_2);
    // bool transfer_bit = false;
    // int multiplier[2 * INTS_IN_DECIMAL] = {0, };
    // int subtotal[2 * INTS_IN_DECIMAL] = {0, };
    // copy_arr_ints(value_1.bits, multiplier, INTS_IN_DECIMAL);
    s21_double_decimal multiplier = {0,};
    s21_double_decimal subtotal = {0,};
    convert_decimal_to_double_decimal(value_1, &multiplier);


    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        for (int j = 0; j < BITS_IN_INT; j++) {
            if (get_bit_int(value_2.bits[i], j)) {
                // sum_arr_int(subtotal, multiplier, subtotal, 2 * INTS_IN_DECIMAL);
                sum_arr_int(subtotal.bits, multiplier.bits, subtotal.bits, 2 * INTS_IN_DECIMAL);
            }
            // left_shift_one_arr_int(multiplier, 2 * INTS_IN_DECIMAL);
            left_shift_one_arr_int(multiplier.bits, 2 * INTS_IN_DECIMAL);
        }
    }

    // print_double_decimal(subtotal);
    // print_double_decimal_in_dec(subtotal);
    // copy_arr_ints(subtotal, result->bits, INTS_IN_DECIMAL);
    set_exponent_decimal(result, exp_1 + exp_2);
    double_decimal_to_decimal(subtotal, result);
    // int count_bit = get_width_number_in_bit(subtotal, 2 * INTS_IN_DECIMAL);
    // printf("=> %d\n", count_bit);
    // print_arr_int(subtotal, 4);
    // if (count_bit > INTS_IN_DECIMAL * BITS_IN_INT) {
    //     printf("=> %d\n", count_bit);
    // }
    // printf("get_width_number_in_bit(subtotal, INTS_IN_DECIMAL) = %d\n", get_width_number_in_bit(subtotal, INTS_IN_DECIMAL));
    // if (get_width_number_in_bit(subtotal, 2 * INTS_IN_DECIMAL) > (INTS_IN_DECIMAL * BITS_IN_INT)) {
    //     status = 1;
    // }
    return status;
 
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal remainder = {0,};
    bool status = s21_div_with_remainder(value_1, value_2, result, &remainder);
    if (!status) {
        s21_decimal fraction = {0,};
        int count = get_width_number_in_bit(remainder.bits, INTS_IN_DECIMAL);
        for (int i = 0; i < (INTS_IN_DECIMAL * BITS_IN_INT - count); i++) {
            left_shift_decimal_ints(&remainder);
        }
        s21_div_with_remainder(remainder, value_2, &fraction, &(s21_decimal){{0,}});
        // print_decimal(fraction);
        // printf("%u\n", fraction.bits[2]);
    }
    // print_decimal(remainder);
    // printf("%u\n", remainder.bits[0]);
    return status;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal quotient = {0,};
    s21_div_with_remainder(value_1, value_2, &quotient, result);
    return 1;
}

// <
int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
    bool less = true;
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        if (value_1.bits[i] > value_2.bits[i]) {
            less = false;
            break;
        } else if (value_1.bits[i] < value_2.bits[i]) {
            less = true;
            break;
        }
    }
    return less;
}

// >
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
    bool less = false;
    for (int i = INTS_IN_DECIMAL - 1; i >= 0; i--) {
        if ((unsigned)(value_1.bits[i]) < (unsigned)(value_2.bits[i])) {
            less = false;
            break;
        } else if ((unsigned)(value_1.bits[i]) > (unsigned)(value_2.bits[i])) {
            less = true;
            break;
        }
    }
    return less;
}

// ==
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
    bool equal = true;
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        if (value_1.bits[i] != value_2.bits[i]) {
            equal = false;
            break;
        }
    }
    return equal;
}

// !=
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
    return !s21_is_equal(value_1, value_2);
}
// 79228162237563176487900676095
// * 15
// 1.1884224335634E+30
// 1.1884224335634E+30


//   7.9228162514264 E+28
// * 1.8446744073710 E+19
//   1.4615016373309 E+48

//   7.9228162514264 E+28
// * 7.9228162514264 E+28
//   6.2771017353867 E+57