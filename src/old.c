#include "s21_decimal.h"
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>


#define BITS_IN_INT 31          // кол-во бит в инте
#define INTS_IN_DECIMAL 3       // кол-во INT хранящих значащее число
#define META_NUMBER_INT 3       // номер INT хранящий знак и порядок в decimal
#define BIT_SIGN BITS_IN_INT    // номер бита знака в INT
#define BIT_EXP_START 16        // номер бита с которого хранится экспонента
#define BIT_EXP_END 23          // номер юита до которого хранится экспонента
#define EXP_MAX 28              // максимальное значение экспоненты
#define EXP_MIN 0               // минимальное значение экспоненты

bool sum_decimal(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
bool sum_arr_ints(int* value_1, int* value_2, int* result);
bool sum_ints(int value_1, int value_2, int* result, bool transfer_bit);

bool left_shift_ints(int* number, int offset);
void copy_ints(int* src, int* dst);
int get_sign(s21_decimal value);
void set_sign(s21_decimal* value, int sign);
int get_sum_exponent(s21_decimal value_1, s21_decimal value_2);
int get_exponent(s21_decimal value);
void set_exponent(s21_decimal* value, int exp);

// РАБОТА С БИТАМИ - нумерация с нуля
bool get_bit_decimal(s21_decimal value, int byte, int ind);
bool set_bit_decimal(s21_decimal* value, int byte, int ind);
bool reset_bit_decimal(s21_decimal* value, int byte, int ind);
bool get_bit_int(int number, int ind);
bool set_bit_int(int* number, int ind);
bool reset_bit_int(int* number, int ind);

// ФУНКЦИИ ВЫВОДА 
void print_decimal(s21_decimal value);
void print_ints(int* value);
void print_byte(int number);


int main() {
    s21_decimal a = {{0, 3, 1,  5 << 16}};
    s21_decimal b = {{0, 32, 1,  9 << 16}};
    s21_decimal c = {{0, 0, 0,  0}};
    a.bits[0] = 255;
    b.bits[0] = 3223423;
    s21_mul(a, b, &c);
    print_decimal(a);
    print_decimal(b);
    print_decimal(c);

    // printf("exp_a = %u\n", get_exponent(a));
    // printf("exp_b = %u\n", get_exponent(b));
    // printf("exp_c = %u\n", get_exponent(c));
    // printf("%u\n", get_sum_exponent(a, b));
    // printf("axp = %u\n", get_exponent(a));
    // printf("a*b = %u\n", c.bits[0]);
    // printf("%u\n", INT_MAX);
    // printf("%u\n", UINT_MAX);
    
    // // print_decimal(a);
    // // print_decimal(b);
    // // sum_decimal(a, b, &c);
    // int d[3] = {2147483647, 3, 2007483647};
    // int c[3] = {0, 0, 0};

    // copy_ints(d, c);
    // printf("transfer = %d\n", mul(c, 1));
    // print_ints(d);
    // print_ints(c);
    return 0;
}


int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    ArithmeticStatus status = OK;
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
    int exp = 0;
    sum_ints(exp_1, exp_2, &exp, false);
    return exp;
}

int get_exponent(s21_decimal value) {
    return (value.bits[NUMBER_INT_META] << (BITS_IN_INT - BIT_EXP_END)) >> (BITS_IN_INT - BIT_EXP_END + BIT_EXP_START);
}

void set_exponent(s21_decimal* value, int exp) {
    int i = 0;
    value->bits[NUMBER_INT_META] &= ~((~i) << (BITS_IN_INT - BIT_EXP_END)) >> (BITS_IN_INT - BIT_EXP_END + BIT_EXP_START);
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

void copy_ints(int* src, int* dst) {
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        dst[i] = src[i];
    }
}

bool left_shift_ints(int* number, int offset) {
    bool transfer = false;
    bool prev_bit = false;
    bool state_bit = false;
    for (int i = 0; i < offset; i++) {
        for (int j = 0; j < INTS_IN_DECIMAL; j++) {
            state_bit = number[j] >> BITS_IN_INT;
            number[j] <<= 1;
            number[j] |= prev_bit;
            prev_bit = state_bit;
        }
        if (!transfer && prev_bit) {
            transfer = true;
        }
        prev_bit = false;
        state_bit = false;
    }
    return transfer;
}

bool right_shift_ints(int* number, int offset) {
    bool transfer = true;
    bool prev_bit = false;
    bool state_bit = false;

    for (int i = 0; i < offset; i++) {
        for (int j = 0; j < INTS_IN_DECIMAL; j++) {
            number[j] >> 1;
            if (j < INTS_IN_DECIMAL - 1) {
                set_bit_int();
                get_bit_int(number[j + 1], 0);
            }
            // state_bit = number[j] >> BITS_IN_INT;
            // number[j] <<= 1;
            // number[j] |= prev_bit;
            // prev_bit = state_bit;
        }

    }
    return transfer;
}


// ФУНКЦИИ СЛОЖЕНИЯ
bool sum_decimal(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    bool transfer_bit = sum_arr_ints(value_1.bits, value_2.bits, result->bits);
    return transfer_bit;
}

bool sum_arr_ints(int* value_1, int* value_2, int* result) {
    bool transfer_bit = false;
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        transfer_bit = sum_ints(value_1[i], value_2[i], &(result)[i], transfer_bit);
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
    return get_bit_int(value.bits[byte], ind);
}

bool get_bit_int(int number, int ind) { 
    return number & (1 << ind);
}

bool set_bit_decimal(s21_decimal* value, int byte, int ind) {  
    return set_bit_int(&(value->bits[byte]), ind);
}

bool set_bit_int(int* number, int ind) {  
    *number |= 1 << ind;
    return true;
}

bool reset_bit_decimal(s21_decimal* value, int byte, int ind) {  
    return reset_bit_int(&(value->bits[byte]), ind);
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

void print_ints(int* value) {
    for (int i = INTS_IN_DECIMAL - 1; i >= 0; i--) {
        print_byte(value[i]);
    }
    printf("\n");
}

void print_byte(int number) {
    unsigned int b;
    for (int i = 31; i >= 0; i--) {
        b = number & (1 << i);
        printf("%d", b >> i);
    }
   
}


