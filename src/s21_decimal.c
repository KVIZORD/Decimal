#include "s21_decimal.h"
#include <stdbool.h>
#include <stdio.h>


void printf_bit(int number);
char *s21_itoa(unsigned long long val, int base);
unsigned int sum_int(int value_1, int value_2, int* result);
bool get_bit(int number, int ind);
bool set_bit(int* number, int ind, bool state);


int main() {
    int a = 343;
    int b = 5682;
    int c = 0;
    // printf("%\n");
    printf_bit(a);
    printf_bit(b);
    sum_int(a, b, &c);
    printf_bit(c);
    printf_bit(a+b);
    printf("%d\n", c);
    printf("%d\n", a+b);
    // printf_bit(a+b);
    // printf("%32s\n", s21_itoa(b, 2));
    return 0;
}


unsigned int sum_int(int value_1, int value_2, int* result) {
    bool transfer = false;
    for (int i = 0; i < 32; i++) {
        bool b_1 = get_bit(value_1, i);
        bool b_2 = get_bit(value_2, i);
        if (b_1 && b_2 && transfer) {
            set_bit(result, i, true);
        } else if ((b_1 && b_2) || (b_1 && transfer) || (b_2 && transfer)) {
            set_bit(result, i, false);
            transfer = true;
        } else if (b_1 || b_2 || transfer) {
            set_bit(result, i, true);
            transfer = false;
        } else {
            set_bit(result, i, false);
        }
    }
    return transfer;
}

void printf_bit(int number) {
    unsigned int b;
    for (int i = 31; i >= 0; i--) {
        b = number & (1 << i);
        printf("%d", b >> i);
    }
    printf("\n");
}

char *s21_itoa(unsigned long long val, int base) {
  static char buf[32] = {0};
  int i = 30;
  if (val == 0) {
    buf[i] = '0';
    i--;
  } else {
    for (; val > 0 && i > 0; i--, val /= base) {
      buf[i] = "0123456789abcdef"[val % base];
    }
  }

  return &(buf[i + 1]);
}


bool get_bit(int number, int ind) { 
    return number & (1 << ind);
}

bool set_bit(int* number, int ind, bool state) {  
    *number &= ~(1 << ind);
    if (state) {
        *number |= 1 << ind;
    }
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

