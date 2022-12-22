#include "s21_decimal.h"
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>




bool sum_ints(int value_1, int value_2, int* result, bool transfer_bit);

int get_sign(s21_decimal value);
void set_sign(s21_decimal* value, int sign);
int get_exponent(s21_decimal value);
void set_exponent(s21_decimal* value, int exp);
int get_sum_exponent(s21_decimal value_1, s21_decimal value_2);
// РАБОТА С БИТАМИ - нумерация с нуля
bool get_bit_decimal(s21_decimal value, int byte, int ind);
bool set_bit_decimal(s21_decimal* value, int byte, int ind);
bool reset_bit_decimal(s21_decimal* value, int byte, int ind);
bool get_bit_int(int number, int ind);
bool set_bit_int(int* number, int ind);
bool reset_bit_int(int* number, int ind);
// ФУНКЦИИ ВЫВОДА 
void print_decimal(s21_decimal value);
void print_byte(int number);


int main() {
    s21_decimal a = {{0, 0, 0,  5 << 16}};
    s21_decimal b = {{0, 0, 0,  9 << 16}};
    s21_decimal c = {{0, 0, 0,  0}};
    a.bits[0] = 32423;
    b.bits[0] = 234313212;
    s21_mul(a, b, &c);
    print_decimal(a);
    print_decimal(b);
    print_decimal(c);

    printf("%u\n", c.bits[0]);
    printf("%u\n", a.bits[0] + b.bits[0]);
    
    return 0;
}


int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int status = 0;
    int offset_value[3] = {0, 0, 0};
    // Знак числа
    if (get_sign(value_1) != get_sign(value_2)) {
        set_sign(result, 1);
    }
    // Степень числа
    int exp = get_sum_exponent(value_1, value_2);
    if (exp > EXP_MAX) {
        status = INF_NEGAT;
    } else {
        set_exponent(result, exp);
    }
    // Умножение числа
    copy_ints(value_1.bits, offset_value);
    for (int i = 0; i < INTS_IN_DECIMAL && status == OK; i++) {
        for (int j = 0; j <= BITS_IN_INT; j++) {
            if (get_bit_int(value_2.bits[i], j)) {
                sum_arr_ints(result->bits, offset_value, result->bits);
            }
            left_shift_ints(offset_value, 1);
        }
    }


    return status;
}


int get_sum_exponent(s21_decimal value_1, s21_decimal value_2) {
    int exp_1 = get_exponent(value_1);
    int exp_2 = get_exponent(value_2);
    int exp = exp_1 + exp_2;
    return exp;
}

int get_exponent(s21_decimal value) {
    return (value.bits[META_NUMBER_INT] << (BITS_IN_INT - BIT_EXP_END)) >> (BITS_IN_INT - BIT_EXP_END + BIT_EXP_START);
}

void set_exponent(s21_decimal* value, int exp) {
    int i = 0;
    value->bits[META_NUMBER_INT] &= ~((~i) << (BITS_IN_INT - BIT_EXP_END)) >> (BITS_IN_INT - BIT_EXP_END + BIT_EXP_START);
    value->bits[META_NUMBER_INT] |= exp << BIT_EXP_START;
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


// ФУНКЦИИ СЛОЖЕНИЯ
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    bool transfer_bit = false;
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        transfer_bit = sum_ints(value_1.bits[i], value_2.bits[i], &(result->bits[i]), transfer_bit);
    }
    return transfer_bit;
}

bool sum_ints(int value_1, int value_2, int* result, bool transfer_bit) {
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
bool get_bit_decimal(s21_decimal value, int byte, int ind) { 
    return value.bits[byte] & (1 << ind);
}

bool set_bit_decimal(s21_decimal* value, int byte, int ind) {
    value->bits[byte] |= 1 << ind;
    return true;
}

bool reset_bit_decimal(s21_decimal* value, int byte, int ind) {
    value->bits[byte] &= ~(1 << ind);
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


// ФУНКЦИИ ВЫВОДА
void print_decimal(s21_decimal value) {
    for (int i = INTS_IN_DECIMAL; i >= 0; i--) {
        print_byte(value.bits[i]);
    }
    printf("\n");
}

void print_byte(int number) {
    for (int i = BITS_IN_INT; i >= 0; i--) {
        printf("%d", (number & (1 << i)) >> i);
    }
}

