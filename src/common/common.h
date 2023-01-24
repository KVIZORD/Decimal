#pragma once
#ifndef SRC_COMMON_COMMON_H_
#define SRC_COMMON_COMMON_H_
#include "../s21_decimal.h"

#include <stdio.h>
#include <stdbool.h>


// common.c
int get_width_number_bits_non_blunk(int* ints, int count_int);
int is_zero_decimal(s21_decimal value);
int is_zero_double_decimal(s21_double_decimal value);
void clear_decimal(s21_decimal* value);
void clear_double_decimal(s21_double_decimal* value);
void clear_full_decimal(s21_decimal* value);
int float_to_scientific_notation_base_10(float num, int len_mantissa, int* mantissa, int* exp);

// arithmetic.c
int sum_int(int number_1, int number_2, int* result, bool transfer_bit);
int sum_ints(int* number_1, int* number_2, int* result, int count_int);
void sub_ints(int* value_1, int* value_2, int* result, int count_int);
int mul_ints(int* value_1, int* value_2, int* result, int count_int);
int is_greater_ints(int* value_1, int* value_2, int count_int);
int is_equal_ints(int* value_1, int* value_2, int count_int);
int add_decimal(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int sub_decimal(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int div_decimal_with_remainder(s21_decimal dividend, s21_decimal divisor, s21_decimal *quotient, s21_decimal *remainder);
int div_double_decimal_with_remainder(s21_double_decimal dividend, s21_double_decimal divisor, s21_double_decimal *quotient, s21_double_decimal *remainder);
int div_double_decimal(s21_double_decimal dividend, s21_double_decimal divisor, s21_double_decimal *result);
void mul_double_decimal(s21_double_decimal value_1, s21_double_decimal value_2, s21_double_decimal* result);
void casting_exp_double_decimal(s21_double_decimal value, s21_double_decimal* result, int exp_new);

// converting.c
void convert_decimal_to_twos_complement(s21_decimal *value);
void convert_decimal_to_ones_complement(s21_decimal* value);
void convert_double_decimal_to_twos_complement(s21_double_decimal* value);
void convert_double_decimal_to_ones_complement(s21_double_decimal* value);
void convert_ints_to_twos_complement(int* value, int count_int);
void convert_ints_to_ones_complement(int* value, int count_int);
int double_decimal_to_decimal(s21_double_decimal src, s21_decimal* dst);
void decimal_to_double_decimal(s21_decimal src, s21_double_decimal* dst);
int normalization_decimal(s21_decimal* value_1, s21_decimal* value_2);
int normalization_double_decimal(s21_double_decimal* value_1, s21_double_decimal* value_2);
void change_exp(s21_decimal* value, int exp);
int bank_round_decimal(s21_decimal* value, int remainder);

// copy.c
int copy_decimal(s21_decimal src, s21_decimal* dst);
int copy_double_decimal(s21_double_decimal src, s21_double_decimal* dst);
int copy_ints(int* src, int* dst, int count_int);

// get_set_exp_and_sign.c
int get_exp(int number);
int set_exp(int* number, int exp);
int get_exp_decimal(s21_decimal value);
int set_exp_decimal(s21_decimal* value, int exp);
int get_exp_double_decimal(s21_double_decimal value);
int set_exp_double_decimal(s21_double_decimal* value, int exp);
int get_sign(int number);
int set_sign(int* number, int sign);
int get_sign_decimal(s21_decimal value);
int set_sign_decimal(s21_decimal* value, int sign);
int get_sign_double_decimal(s21_double_decimal value);
int set_sign_double_decimal(s21_double_decimal* value, int sign);
void change_exp_decimal(s21_decimal* value, int exp);
int get_sign_float(float number);
int set_sign_float(float* number, int sign);

// shift.c
int left_shift_decimal(s21_decimal* value);
int left_shift_double_decimal(s21_double_decimal* value);
int left_shift_ints(int* ints, int num_int);
int right_shift_decimal(s21_decimal* value);
int right_shift_double_decimal(s21_double_decimal* value);
int right_shift_ints(int* ints, int num_int);

// get_set_bit.c
int get_bit_decimal(s21_decimal value, int num_int, int ind);
int set_bit_decimal(s21_decimal* value, int num_int, int ind);
int reset_bit_decimal(s21_decimal* value, int num_int, int ind);
int get_bit_double_decimal(s21_double_decimal value, int num_int, int ind);
int set_bit_double_decimal(s21_double_decimal* value, int num_int, int ind);
int reset_bit_double_decimal(s21_double_decimal* value, int num_int, int ind);
int get_bit_ints(int* value, int num_int, int ind);
int set_bit_ints(int* value, int num_int, int ind);
int reset_bit_ints(int* value, int num_int, int ind);
int get_bit_int(int number, int ind);
int set_bit_int(int* number, int ind);
int reset_bit_int(int* number, int ind);

// print.c
void print_binary_int(int number);
void print_binary_ints(int* ints, int count_int);
void print_decimal(s21_decimal value);
void print_double_decimal(s21_double_decimal value);
void print_decimal_in_dec(s21_decimal value);
void print_double_decimal_in_dec(s21_double_decimal value);
__uint128_t decimal_to_int128(s21_decimal value);
__uint128_t double_decimal_to_int128(s21_double_decimal value);

#endif // SRC_COMMON_COMMON_H_
