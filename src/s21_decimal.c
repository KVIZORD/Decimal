#include "s21_decimal.h"
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>


#define BITS_IN_INT 32
#define COUNT_INT_IN_DECIMAL 3
#define BIT_SIGN (BITS_IN_INT - 1)
#define BIT_EXP_START 16
#define BIT_EXP_END 23

void printf_decimal(s21_decimal value);
void printf_byte(int number);
char *s21_itoa(unsigned long long val, int base);
bool sum_decimal(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
bool sum_int(int value_1, int value_2, int* result, bool transfer_bit);
bool get_bit(int number, int ind);
bool set_bit(int* number, int ind);
bool reset_bit(int* number, int ind);
void print_int128(__int128_t value);

int main() {
    s21_decimal a = {{0, 2, 2, 77}};
    s21_decimal b = {{0, 1, 1,  1}};
    s21_decimal c = {{0, 0, 0,  0}};
    
    sum_decimal(a, b, &c);
    printf_decimal(a);
    printf_decimal(b);
    printf_decimal(c);
    // int a = 332443;
    // int b = 5682;
    // int c = 12340;
    // printf("%\n");
    // printf_bit(a);
    // printf_bit(b);
    // sum_int(a, b, &c, false);
    // printf_bit(c);
    // printf_bit(a+b);
    // printf("%d\n", c);
    // printf("%d\n", a+b);
    // printf_bit(a+b);
    // printf("%32s\n", s21_itoa(b, 2));
    return 0;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    // for (int i = BIT_EXP_END; i <= BIT_EXP_START; i++) {

    // }

    
}

bool sum_decimal(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    bool transfer_bit = false;
    for (int i = 0; i < COUNT_INT_IN_DECIMAL; i++) {
        transfer_bit = sum_int(value_1.bits[i], value_2.bits[i], &(result->bits[i]), transfer_bit);
    }
    return transfer_bit;
}

bool sum_int(int value_1, int value_2, int* result, bool transfer_bit) {
    for (int i = 0; i < 32; i++) {
        bool b_1 = get_bit(value_1, i);
        bool b_2 = get_bit(value_2, i);
        if (b_1 && b_2 && transfer_bit) {
            set_bit(result, i);
        } else if ((b_1 && b_2) || (b_1 && transfer_bit) || (b_2 &&transfer_bit)) {
            reset_bit(result, i);
            transfer_bit = true;
        } else if (b_1 || b_2 || transfer_bit) {
            set_bit(result, i);
            transfer_bit = false;
        } else {
            reset_bit(result, i);
        }
    }
    return transfer_bit;
}

void printf_byte(int number) {
    unsigned int b;
    for (int i = 31; i >= 0; i--) {
        b = number & (1 << i);
        printf("%d", b >> i);
    }
   
}

void printf_decimal(s21_decimal value) {
    for (int i = 0; i < COUNT_INT_IN_DECIMAL; i++) {
        printf_byte(value.bits[i]);
    }
    printf("\n");
}

bool get_bit(int number, int ind) { 
    return number & (1 << ind);
}

bool set_bit(int* number, int ind) {  
    *number |= 1 << ind;
    return true;
}

bool reset_bit(int* number, int ind) {  
    *number &= ~(1 << ind);
    return true;
}

// void normalization(s21_decimal* value, int exponent) {

// }

// void numbers_normalization(s21_decimal* value_1, s21_decimal* value_2) {
//     if (value_1->exponent < value_2->exponent) {
//         int offset = value_2->exponent - value_1->exponent;
//         normalization(value_1, offset)
//     } else if (value_1->exponent > value_2->exponent) {
//         int offset = value_1->exponent - value_2->exponent;
//         normalization(value_2, offset)
//     }
// }

// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//     for (int i = 0; i < 3; i++) {
//         result->bits[i] = value_1.bits[i] & value_2.bits[i];
//         if (get_bit(value_1.bits[i], 31) && get_bit(value_2.bits[i], 31)) {

//         }
//     }

//     return 0;
// }

