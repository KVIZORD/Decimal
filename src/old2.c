#include "s21_decimal.h"
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>




bool sum_ints(int value_1, int value_2, int* result, bool transfer_bit);
bool sum_arr_int(int* value_1, int* value_2, int* result, int count_int);
int sub_arr_int(int* value_1, int* value_2, int* result, int count_int);

bool copy_arr_ints(int* src, int* dst, int count_int_src);
bool left_shift_one_arr_int(int* ints, int num_int);
bool left_shift_arr_int(int* ints, int num_int, unsigned int offset);

int get_sign(s21_decimal value);
void set_sign(s21_decimal* value, int sign);
int get_exponent(s21_decimal value);
void set_exponent(s21_decimal* value, int exp);
int get_sum_exponent(s21_decimal value_1, s21_decimal value_2);

// РАБОТА С БИТАМИ - нумерация с нуля
bool get_bit_decimal(s21_decimal value, int num_int, int ind);
bool set_bit_decimal(s21_decimal* value, int num_int, int ind);
bool reset_bit_decimal(s21_decimal* value, int num_int, int ind);
bool get_bit_int(int number, int ind);
bool set_bit_int(int* number, int ind);
bool reset_bit_int(int* number, int ind);
bool get_bit_arr_int(int* value, int num_int, int ind);
bool set_bit_arr_int(int* value, int num_int, int ind);
bool reset_bit_arr_int(int* value, int num_int, int ind);
int get_width_number_in_bit(int* ints, int count_int);

// ФУНКЦИИ ВЫВОДА 
void print_decimal(s21_decimal value);
void print_arr_int(int* value, int count_int);
void print_binary_int(int number);



int main() {
    s21_decimal a = {{0, 0, 0,  5 << 16}};
    s21_decimal b = {{0, 0, 0,  9 << 16}};
    s21_decimal c = {{0, 0, 0,  0}};
    a.bits[0] = 20323;
    // a.bits[1] = 2;
    b.bits[0] = 2342;
    // b.bits[1] = 2193132120;
    // a.bits[2] = 2193132120;
    // b.bits[0] = 234313212;
    // print_decimal(a);
    // print_decimal(b);
    sub_arr_int(a.bits, b.bits, c.bits, 3);
    // s21_mul(a, b, &c);
    print_decimal(a);
    print_decimal(b);
    print_decimal(c);
    // printf("%u\n", left_shift_arr_int(a.bits, 3, 82));
    // print_decimal(a);
    printf("%u\n", c.bits[0]);
    // printf("%u\n", a.bits[0] - b.bits[0]);
    
    return 0;
}


// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//     int status = 0;
//     int offset_value[3] = {0, 0, 0};
//     // Знак числа
//     if (get_sign(value_1) != get_sign(value_2)) {
//         set_sign(result, 1);
//     }
//     // Степень числа
//     int exp = get_sum_exponent(value_1, value_2);
//     if (exp > EXP_MAX) {
//         status = INF_NEGAT;
//     } else {
//         set_exponent(result, exp);
//     }
//     // Умножение числа
//     // copy_ints(value_1.bits, offset_value);
//     for (int i = 0; i < INTS_IN_DECIMAL && status == OK; i++) {
//         for (int j = 0; j <= BITS_IN_INT; j++) {
//             if (get_bit_int(value_2.bits[i], j)) {
//                 sum_arr_ints(result->bits, offset_value, result->bits);
//             }
//             left_shift_ints(offset_value, 1);
//         }
//     }
//     return status;
// }

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int status = 0;
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
    int count_bit = get_width_number_in_bit(subtotal, 2 * INTS_IN_DECIMAL);
    printf("=> %d\n", count_bit);
    print_arr_int(subtotal, 4);
    if (count_bit > INTS_IN_DECIMAL * BITS_IN_INT) {
        printf("=> %d\n", count_bit);
    }
    return status;
 
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
    return (value.bits[NUMBER_INT_META] << (BITS_IN_INT - 1 - BIT_EXP_END)) >> (BITS_IN_INT - 1 - BIT_EXP_END + BIT_EXP_START);
}

void set_exponent(s21_decimal* value, int exp) {
    int i = 0;
    value->bits[NUMBER_INT_META] &= ~((~i) << (BITS_IN_INT - 1 - BIT_EXP_END)) >> (BITS_IN_INT - 1 - BIT_EXP_END + BIT_EXP_START);
    value->bits[NUMBER_INT_META] |= exp << BIT_EXP_START;
}

int get_sign(s21_decimal value) {
    return get_bit_decimal(value, INTS_IN_DECIMAL, BIT_SIGN);
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
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    bool transfer_bit = false;
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        transfer_bit = sum_ints(value_1.bits[i], value_2.bits[i], &(result->bits[i]), transfer_bit);
    }
    return transfer_bit;
}

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
bool get_bit_decimal(s21_decimal value, int num_int, int ind) { 
    return value.bits[num_int] & (1 << ind);
}

bool set_bit_decimal(s21_decimal* value, int num_int, int ind) {    
    value->bits[num_int] |= 1 << ind;
    return true;
}

bool reset_bit_decimal(s21_decimal* value, int num_int, int ind) {
    value->bits[num_int] &= ~(1 << ind);
    return true;
}

bool get_bit_int(int number, int ind) { 
    return number & (1 << ind);
}

bool set_bit_int(int* number, int ind) {  
    *number |= 1 << ind;
    return true;
}

bool reset_bit_int(int* number, int ind) {  
    *number &= ~(1 << ind);
    return true;
}

bool get_bit_arr_int(int* value, int num_int, int ind) { 
    return get_bit_int(value[num_int], ind);
}

bool set_bit_arr_int(int* value, int num_int, int ind) {
    set_bit_int(&(value[num_int]), ind);
    return true;
}

bool reset_bit_arr_int(int* value, int num_int, int ind) {
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

