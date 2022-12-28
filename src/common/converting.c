#include "services.h"

/// @brief Конверирование мантисы DECIMAL в дополнительный код
/// @param value Указатель на конвертируемое число
void convert_decimal_to_twos_complement(s21_decimal *value) {
    convert_decimal_to_ones_complement(value);
    s21_decimal one = {{1, 0, 0, 0}};
    set_sign_decimal(&one, get_sign_decimal(*value));
    set_exponent_decimal(&one, get_exponent_decimal(*value));
    s21_add(*value, one, value);
}

/// @brief Конверирование мантисы DECIMAL в обратный код
/// @param value Указатель на конвертируемое число
void convert_decimal_to_ones_complement(s21_decimal* value) {
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        value->bits[i] = ~(value->bits[i]);
    }
}

/// @brief Приведение двух DECIMAL к одному значению экспоненты
/// @param value_1 Указатель на первое число DECIMAL
/// @param value_2 Указатель на второе число DECIMAL
/// @return 0 - успешно, 1 - ошибка преобразования
int normalization_decimal(s21_decimal* value_1, s21_decimal* value_2) {
    Status status = STATUS_OK;
    int exp_1 = get_exponent_decimal(*value_1);
    int exp_2 = get_exponent_decimal(*value_2);
    if (exp_1 > EXP_MAX || exp_2 > EXP_MAX) {
        status = STATUS_ERR;
    }
    if (status == STATUS_OK && exp_1 > exp_2) {
        shift_point(value_2, exp_1);
        exp_2 = get_exponent_decimal(*value_2);
        shift_point(value_1, exp_2);
        exp_1 = get_exponent_decimal(*value_1);
    }
    if (status == STATUS_OK && exp_1 < exp_2) {
        shift_point(value_1, exp_2);
        exp_1 = get_exponent_decimal(*value_1);
        shift_point(value_2, exp_1);
        exp_2 = get_exponent_decimal(*value_2);
    }
    if (exp_1 != exp_2) {
        status = STATUS_ERR;
    }
    return status;
}

/// @brief Банковское окуругление DECIMAL
/// @param value Указатель на число для округления
/// @param right_bit Потерянный бит справа
void round_banking(s21_decimal* value, bool right_bit) {
    if (right_bit) {
        s21_decimal one = {{1, 0, 0, 0}};
        set_sign_decimal(&one, get_sign_decimal(*value));
        set_exponent_decimal(&one, get_exponent_decimal(*value));
        s21_add(*value, one, value);
    }
}

/// @brief Конвертация DECIMAL в DOUBLE DECIMAL
/// @param src Число DECIMAL
/// @param dst Число DOUBLE DECIMAL
void convert_decimal_to_double_decimal(s21_decimal src, s21_double_decimal* dst) {
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        dst->bits[i] = src.bits[i];
    }
    dst->bits[2 * INTS_IN_DECIMAL] = src.bits[INTS_IN_DECIMAL];
}


/// @brief Конвертация числа DOUBLE DECIMAL в DECIMAL
/// @param src Число DECIMAL
/// @param dst Число DOUBLE DECIMAL
void double_decimal_to_decimal(s21_double_decimal src, s21_decimal* dst) {
    while (get_width_number_in_bit(src.bits, 2 * INTS_IN_DECIMAL) > INTS_IN_DECIMAL * BITS_IN_INT) {
        // print_double_decimal(subtotal);
        // print_double_decimal_in_dec(subtotal);
        break;
    }
    print_decimal(*dst);
    // for (int i = 0; i < INTS_IN_DECIMAL; i++) {
    //     dst->bits[i] = src.bits[i];
    // }
    // dst->bits[2 * INTS_IN_DECIMAL] = src.bits[INTS_IN_DECIMAL];
}
