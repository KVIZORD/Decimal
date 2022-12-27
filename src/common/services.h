#pragma once
#ifndef SRC_COMMON_SERVICES_H_
#define SRC_COMMON_SERVICES_H_
#include "../s21_decimal.h"

#include <stdio.h>
#include <stdbool.h>

int s21_div_with_remainder(s21_decimal dividend, s21_decimal divisor, s21_decimal *quotient, s21_decimal *remainder);
void s21_copy_decimal(s21_decimal src, s21_decimal* dst);
bool s21_is_zero_decimal(s21_decimal value);
void decimal_clear(s21_decimal* value);
bool copy_arr_ints(int* src, int* dst, int count_int_src);
int sub_arr_int(int* value_1, int* value_2, int* result, int count_int);
bool sum_arr_int(int* value_1, int* value_2, int* result, int count_int);
bool sum_ints(int value_1, int value_2, int* result, bool transfer_bit);
int s21_sum(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// ФУНКЦИИ КОНВЕРТАЦИИ
void round_banking(s21_decimal* value, bool right_bit);
void convert_decimal_to_twos_complement(s21_decimal *value);
void convert_decimal_to_ones_complement(s21_decimal* value);
int normalization_decimal(s21_decimal* value_1, s21_decimal* value_2);

// ФУНКЦИИ ДЛЯ РАБОТЫ С ЗНАКОМ И ЭКСПОНЕНТОЙ - DECIMAL
int get_sum_exponent(s21_decimal value_1, s21_decimal value_2);
int get_exponent(s21_decimal value);
void set_exponent(s21_decimal* value, int exp);
int get_sign(s21_decimal value);
void set_sign(s21_decimal* value, int sign);

// ФУНКЦИИ РАБОТЫ С БИТАМИ - DECIMAL
int get_bit_decimal(s21_decimal value, int num_int, int ind);
int set_bit_decimal(s21_decimal* value, int num_int, int ind);
int reset_bit_decimal(s21_decimal* value, int num_int, int ind);

// ФУНКЦИИ РАБОТЫ С БИТАМИ - DOUBLE DECIMAL
int get_bit_double_decimal(s21_double_decimal value, int num_int, int ind);
int set_bit_double_decimal(s21_double_decimal* value, int num_int, int ind);
int reset_bit_double_decimal(s21_double_decimal* value, int num_int, int ind);

// ФУНКЦИИ РАБОТЫ С БИТАМИ - INT
int get_bit_int(int number, int ind);
int set_bit_int(int* number, int ind);
int reset_bit_int(int* number, int ind);

// ФУНКЦИИ РАБОТЫ С БИТАМИ - МАССИВ INT
int get_bit_arr_int(int* value, int num_int, int ind);
int set_bit_arr_int(int* value, int num_int, int ind);
int reset_bit_arr_int(int* value, int num_int, int ind);
int get_width_number_in_bit(int* ints, int count_int);

// ФУНКЦИИ СДВИГА 
bool right_shift_decimal(s21_decimal* value, bool left_bit);
bool left_shift_decimal_ints(s21_decimal* value);
bool left_shift_arr_int(int* ints, int num_int, unsigned int offset);
bool left_shift_one_arr_int(int* ints, int num_int);
void shift_point(s21_decimal* value, int exp);

// ФУНКЦИИ ВЫВОДА
void print_decimal(s21_decimal value);
void print_arr_int(int* value, int count_int);
void print_binary_int(int number);
void print_decimal_in_dec(s21_decimal value);
__int128_t decimal_to_int128(s21_decimal value);

#endif  // SRC_COMMON_SERVICES_H_
