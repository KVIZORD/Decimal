#include "s21_decimal.h"
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
bool left_shift_decimal_ints(s21_decimal* value);
bool left_shift_one_arr_int(int* ints, int num_int);
bool left_shift_arr_int(int* ints, int num_int, unsigned int offset);
bool right_shift_decimal_ints(s21_decimal* value, bool left_bit);
void decimal_clear(s21_decimal* value);
void bank_round(s21_decimal* value, bool right_bit);

int s21_sum(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int get_sign(s21_decimal value);
void set_sign(s21_decimal* value, int sign);
int get_exponent(s21_decimal value);
void set_exponent(s21_decimal* value, int exp);
int get_sum_exponent(s21_decimal value_1, s21_decimal value_2);
void convert_decimal_to_ones_complement(s21_decimal value, s21_decimal *result);
void convert_decimal_to_twos_complement(s21_decimal value, s21_decimal *result);


int get_bit_decimal(s21_decimal value, int num_int, int ind);
int set_bit_decimal(s21_decimal* value, int num_int, int ind);
int reset_bit_decimal(s21_decimal* value, int num_int, int ind);
int get_bit_double_decimal(s21_double_decimal value, int num_int, int ind);
int set_bit_double_decimal(s21_double_decimal* value, int num_int, int ind);
int reset_bit_double_decimal(s21_double_decimal* value, int num_int, int ind);
int get_bit_int(int number, int ind);
int set_bit_int(int* number, int ind);
int reset_bit_int(int* number, int ind);
int get_bit_arr_int(int* value, int num_int, int ind);
int set_bit_arr_int(int* value, int num_int, int ind);
int reset_bit_arr_int(int* value, int num_int, int ind);
int get_width_number_in_bit(int* ints, int count_int);
// ФУНКЦИИ ВЫВОДА 
void print_decimal(s21_decimal value);
void print_arr_int(int* value, int count_int);
void print_binary_int(int number);


int normalization_decimal(s21_decimal* value_1, s21_decimal* value_2);
void shift_point(s21_decimal* value, int exp);


int main() {
    // 5 << 16
    s21_decimal a = {{0, 0, 0,  1 << 16}};
    s21_decimal b = {{0, 0, 0,  0 << 16}};
    s21_decimal c = {{0, 0, 0,  0}};
    a.bits[0] = 0xffffffff;
    // a.bits[1] = 0xffffffff;
    // a.bits[2] = 0xffffffff;
    b.bits[0] = 1;
    // normalization_decimal(&a, &b);
    s21_add(a, b, &c);
    print_decimal(a);
    print_decimal(b);
    print_decimal(c);
    printf("%u\n", get_exponent(a));
    printf("%u\n", get_exponent(b));
    printf("%u\n", get_exponent(c));
    // printf("sign = %u\n", get_sign(a));
    // return 0;
    // s21_mul(a, b, &c);
    // s21_div(a, b, &c);
    // s21_mod(a, b, &c);
    // print_decimal(a);
    // print_decimal(b);
    // shift_point(&a, 29);
    // printf("%u\n", a.bits[0]);
    // printf("%u\n", get_exponent(a));
    // printf("%u\n", get_sign(c));
    // get_bit_decimal

    // printf("%u\n", a.bits[0] + b.bits[0]);

    return 0;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    // bool transfer_bit = false;
    Status status = normalization_decimal(&value_1, &value_2);
    if (status == STATUS_OK) {
        int sign_2 = get_sign(value_2);
        int sign_1 = get_sign(value_1);
        int exp = get_exponent(value_1);
        if (sign_1 == sign_2) {
            set_sign(result, sign_1);
            set_exponent(result, exp);
            s21_sum(value_1, value_2, result);
        } else if (sign_1) {
            s21_sub(value_2, value_1, result);
        } else if (sign_2) {
            s21_sub(value_1, value_2, result);
        }
    }

    // int exp_1 = get_exponent(value_1);
    // int exp_2 = get_exponent(value_2);

    return status;
}




int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    convert_decimal_to_twos_complement(value_2, &value_2);
    return s21_add(value_1, value_2, result);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int status = 0;
    int exp_1 = get_exponent(value_1);
    int exp_2 = get_exponent(value_2);
    // printf("exp_1 + exp_2 = %d\n", exp_1 + exp_2);
    // bool transfer_bit = false;
    int multiplier[2 * INTS_IN_DECIMAL] = {0, };
    int subtotal[2 * INTS_IN_DECIMAL] = {0, };
    copy_arr_ints(value_1.bits, multiplier, INTS_IN_DECIMAL);

    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        for (int j = 0; j < BITS_IN_INT; j++) {
            if (get_bit_int(value_2.bits[i], j)) {
                sum_arr_int(subtotal, multiplier, subtotal, 2 * INTS_IN_DECIMAL);
            }
            left_shift_one_arr_int(multiplier, 2 * INTS_IN_DECIMAL);
        }
    }

    copy_arr_ints(subtotal, result->bits, INTS_IN_DECIMAL);

    set_exponent(result, exp_1 + exp_2);
    // int count_bit = get_width_number_in_bit(subtotal, 2 * INTS_IN_DECIMAL);
    // printf("=> %d\n", count_bit);
    // print_arr_int(subtotal, 4);
    // if (count_bit > INTS_IN_DECIMAL * BITS_IN_INT) {
    //     printf("=> %d\n", count_bit);
    // }
    // printf("get_width_number_in_bit(subtotal, INTS_IN_DECIMAL) = %d\n", get_width_number_in_bit(subtotal, INTS_IN_DECIMAL));
    if (get_width_number_in_bit(subtotal, 2 * INTS_IN_DECIMAL) > (INTS_IN_DECIMAL * BITS_IN_INT)) {
        status = 1;
    }
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

int s21_sum(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    bool transfer_bit = false;
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        transfer_bit = sum_ints(value_1.bits[i], value_2.bits[i], &(result->bits[i]), transfer_bit);
    }
    // print_decimal(value_1);
    // print_decimal(value_2);
    // print_decimal(*result);
    // printf("transfer_bit = %d\n", transfer_bit);
    int exp = get_exponent(*result);
    // если случилось переполнение и степень больше нуля
    if (transfer_bit && exp > 0) {
        transfer_bit = false;
        bool right_bit = right_shift_decimal_ints(result, true);      // сдвиг вправо - т.е. деление на 2
        printf("before = \n");
        print_decimal(*result);
        bank_round(result, right_bit);
        printf("after = \n");
        print_decimal(*result);
        s21_div(*result, (s21_decimal){{1, 0, 0, 0}}, result);  // деление на 5
        set_exponent(result, exp - 1);                          // уменьшение степени
    }

    return transfer_bit;
}

void bank_round(s21_decimal* value, bool right_bit) {
    printf("right_bit = %d\n", right_bit);
    if (right_bit) {
        s21_add(*value, (s21_decimal){{1, 0, 0, 0}}, value);
    }
}


int s21_div_with_remainder(s21_decimal dividend, s21_decimal divisor, s21_decimal *quotient, s21_decimal *remainder) {
    int status = 0;
    if (!s21_is_zero_decimal(divisor)) {
        s21_decimal tmp = {0,};
        for (int num_int = INTS_IN_DECIMAL - 1; num_int >= 0; num_int--) {
            for (int num_bit = BITS_IN_INT - 1; num_bit >= 0; num_bit--) {
                // сдвиг уменьшаемого числа влево на один разряд
                bool state_bit = get_bit_decimal(dividend, num_int, num_bit);
                left_shift_decimal_ints(&tmp);
                // перенос бита в нулевую позицию сдвинутого числа
                if (state_bit) {
                    set_bit_decimal(&tmp, 0, 0);
                }
                // вычисление остатка
                if (s21_is_greater(tmp, divisor)) {
                    // остататок > 0
                    s21_sub(tmp, divisor, &tmp);
                    set_bit_decimal(quotient, num_int, num_bit);
                } else if (s21_is_equal(tmp, divisor)) {
                    // остататок = 0
                    decimal_clear(&tmp);
                    set_bit_decimal(quotient, num_int, num_bit);
                }
            }
        }
        s21_copy_decimal(tmp, remainder);
    } else {
        status = 3;
    }
    return status;
}

void s21_copy_decimal(s21_decimal src, s21_decimal* dst) {
    for (int i = 0; i <= INTS_IN_DECIMAL; i++) {
        dst->bits[i] = src.bits[i];
    }
}

bool s21_is_zero_decimal(s21_decimal value) {
    bool zero = true;
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        if (value.bits[i]) {
            zero = false;
            break;
        }
    }
    return zero;
}

void decimal_clear(s21_decimal* value) {
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        value->bits[i] = 0;
    }
}

// дополнительный код
void convert_decimal_to_twos_complement(s21_decimal value, s21_decimal *result) {
    convert_decimal_to_ones_complement(value, result);
    s21_add(*result, (s21_decimal){{1, 0, 0, 0}}, result);
}

// обратный код
void convert_decimal_to_ones_complement(s21_decimal value, s21_decimal *result) {
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        result->bits[i] = ~(value.bits[i]);
    }
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

bool left_shift_decimal_ints(s21_decimal* value) {
    return left_shift_one_arr_int(value->bits, INTS_IN_DECIMAL);
}

bool right_shift_decimal_ints(s21_decimal* value, bool left_bit) {
    bool transfer_bit = get_bit_int(value->bits[0], 0);
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        value->bits[i] >>= 1;
        if (i < (INTS_IN_DECIMAL - 1)) {
            if (get_bit_decimal(*value, i + 1, 0)) {
                set_bit_decimal(value, i, BITS_IN_INT - 1);
            } else {
                reset_bit_decimal(value, i, BITS_IN_INT - 1);
            }
        }
    }
    if (left_bit) {
        set_bit_decimal(value, INTS_IN_DECIMAL - 1, BITS_IN_INT - 1);
    } else {
        reset_bit_decimal(value, INTS_IN_DECIMAL - 1, BITS_IN_INT - 1);
    }
    return transfer_bit;
}

bool left_shift_arr_int(int* ints, int num_int, unsigned int offset) {
    bool transfer_bit = false;
    for (unsigned int i = 0; i < offset; i++) {
       transfer_bit = left_shift_one_arr_int(ints, num_int) ? true : transfer_bit;
    }
    return transfer_bit;
}

bool left_shift_one_arr_int(int* ints, int num_int) {
    bool transfer_bit = get_bit_int(ints[num_int - 1], BITS_IN_INT - 1);
    for (int i = num_int - 1; i >= 0; i--) {
        ints[i] <<= 1;
        if (i > 0) {
            ints[i] |= ((ints[i - 1] >> (BITS_IN_INT - 1)) & 1);
        }
    }
    return transfer_bit;
}


bool copy_arr_ints(int* src, int* dst, int count_int_src) {
    for (int i = 0; i < count_int_src; i++) {
        dst[i] = src[i];
    }
    return true;
}



int get_sum_exponent(s21_decimal value_1, s21_decimal value_2) {
    int exp_1 = get_exponent(value_1);
    int exp_2 = get_exponent(value_2);
    int exp = exp_1 + exp_2;
    return exp;
}

int get_exponent(s21_decimal value) {
    return (value.bits[META_NUMBER_INT - 1] << (BITS_IN_INT - 1 - BIT_EXP_END)) >> (BITS_IN_INT - 1 - BIT_EXP_END + BIT_EXP_START);
}

void set_exponent(s21_decimal* value, int exp) {
    int i = 0;
    value->bits[META_NUMBER_INT - 1] &= ~((~i) << (BITS_IN_INT - 1 - BIT_EXP_END)) >> (BITS_IN_INT - 1 - BIT_EXP_END + BIT_EXP_START);
    value->bits[META_NUMBER_INT - 1] |= exp << BIT_EXP_START;
}

int get_sign(s21_decimal value) {
    return get_bit_decimal(value, INTS_IN_DECIMAL, BIT_SIGN - 1);
}

void set_sign(s21_decimal* value, int sign) {
    if (sign) {
        set_bit_decimal(value, INTS_IN_DECIMAL, BIT_SIGN);
    } else {
        reset_bit_decimal(value, INTS_IN_DECIMAL, BIT_SIGN);
    }
}

int sub_arr_int(int* value_1, int* value_2, int* result, int count_int) {
  for (int i = 0; i < count_int; i++) {
    value_2[i] = ~(value_2[i]);
    result[i] = 0;
  }
  set_bit_arr_int(result, 0, 0);  
  bool transfer_bit = sum_arr_int(value_2, result, value_2, count_int);
  reset_bit_arr_int(result, 0, 0);
  transfer_bit = sum_arr_int(value_1, value_2, result, count_int);
  return transfer_bit;
}


// ФУНКЦИИ СЛОЖЕНИЯ
bool sum_arr_int(int* value_1, int* value_2, int* result, int count_int) {
    bool transfer_bit = false;
    for (int i = 0; i < count_int; i++) {
        transfer_bit = sum_ints(value_1[i], value_2[i], &(result)[i], transfer_bit);
    }
    return transfer_bit;
}

bool sum_ints(int value_1, int value_2, int* result, bool transfer_bit) {
    // побитовое сложение
    for (int i = 0; i < 32; i++) {
        bool b_1 = get_bit_int(value_1, i);
        bool b_2 = get_bit_int(value_2, i);
        if (b_1 && b_2 && transfer_bit) {
            set_bit_int(result, i);
        } else if ((b_1 && b_2) || (b_1 && transfer_bit) || (b_2 &&transfer_bit)) {
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


// РАБОТА С БИТАМИ - нумерация с нуля
int get_bit_decimal(s21_decimal value, int num_int, int ind) { 
    return (value.bits[num_int] >> ind) & 1 ;
}

int set_bit_decimal(s21_decimal* value, int num_int, int ind) {    
    value->bits[num_int] |= 1 << ind;
    return true;
}

int reset_bit_decimal(s21_decimal* value, int num_int, int ind) {
    value->bits[num_int] &= ~(1 << ind);
    return true;
}

int get_bit_double_decimal(s21_double_decimal value, int num_int, int ind) { 
    return value.bits[num_int] & (1 << ind);
}

int set_bit_double_decimal(s21_double_decimal* value, int num_int, int ind) {    
    value->bits[num_int] |= 1 << ind;
    return true;
}

int reset_bit_double_decimal(s21_double_decimal* value, int num_int, int ind) {
    value->bits[num_int] &= ~(1 << ind);
    return true;
}

int get_bit_int(int number, int ind) { 
    return number & (1 << ind);
}

int set_bit_int(int* number, int ind) {  
    *number |= 1 << ind;
    return true;
}

int reset_bit_int(int* number, int ind) {  
    *number &= ~(1 << ind);
    return true;
}

int get_bit_arr_int(int* value, int num_int, int ind) { 
    return get_bit_int(value[num_int], ind);
}

int set_bit_arr_int(int* value, int num_int, int ind) {
    set_bit_int(&(value[num_int]), ind);
    return true;
}

int reset_bit_arr_int(int* value, int num_int, int ind) {
    reset_bit_int(&(value[num_int]), ind);
    return true;
}

int get_width_number_in_bit(int* ints, int count_int) {
    int width = 0;
    for (int i = 0; i < count_int; i++) {
        for (int j = 0; j < BITS_IN_INT; j++) {
            if (get_bit_int(ints[i], j)) {
                width = i * BITS_IN_INT + j + 1;
            }
        }
    }
    return width;
}



// ФУНКЦИИ ВЫВОДА
void print_decimal(s21_decimal value) {
    for (int i = INTS_IN_DECIMAL; i >= 0; i--) {
        print_binary_int(value.bits[i]);
        printf("|");
    }
    printf("\n");
}

void print_arr_int(int* value, int count_int) {
    for (int i = count_int - 1; i >= 0; i--) {
        print_binary_int(value[i]);
    }
    printf("\n");
}

void print_binary_int(int number) {
    for (int i = BITS_IN_INT - 1; i >= 0; i--) {
        printf("%d", ((unsigned int)number & (1 << i)) >> i);
    }
}


// +++
int normalization_decimal(s21_decimal* value_1, s21_decimal* value_2) {
    Status status = STATUS_OK;
    int exp_1 = get_exponent(*value_1);
    int exp_2 = get_exponent(*value_2);
    if (exp_1 > EXP_MAX || exp_2 > EXP_MAX) {
        status = STATUS_ERR;
    }
    if (status == STATUS_OK && exp_1 > exp_2) {
        shift_point(value_2, exp_1);
        exp_2 = get_exponent(*value_2);
        shift_point(value_1, exp_2);
        exp_1 = get_exponent(*value_1);
    }
    if (status == STATUS_OK && exp_1 < exp_2) {
        shift_point(value_1, exp_2);
        exp_1 = get_exponent(*value_1);
        shift_point(value_2, exp_1);
        exp_2 = get_exponent(*value_2);
    }
    if (exp_1 != exp_2) {
        status = STATUS_ERR;
    }
    return status;
}

// +++
void shift_point(s21_decimal* value, int exp) {
    s21_decimal tmp = {{0, 0, 0, 0}};
    int exp_old = get_exponent(*value);
    int exp_new = exp_old;
    for (int i = exp_old; i < exp; i++) {
        bool status = s21_mul(*value, (s21_decimal){{10, 0, 0, 0}}, &tmp);
        if (status) {
            break;
        }
        exp_new += 1;
        s21_copy_decimal(tmp, value);
    }
    for (int i = exp; i < exp_old; i++) {
        bool status = s21_div(*value, (s21_decimal){{10, 0, 0, 0}}, &tmp);
        s21_copy_decimal(tmp, value);
        if (status) {
            break;
        }
        exp_new -= 1;
        s21_copy_decimal(tmp, value);
    }
    set_exponent(value, exp_new);
}

