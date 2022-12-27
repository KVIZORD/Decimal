#include "services.h"

/// @brief Конверирование мантисы DECIMAL в дополнительный код
/// @param value Указатель на конвертируемое число
void convert_decimal_to_twos_complement(s21_decimal *value) {
    convert_decimal_to_ones_complement(value);
    s21_decimal one = {{1, 0, 0, 0}};
    set_sign(&one, get_sign(*value));
    set_exponent(&one, get_exponent(*value));
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

/// @brief Банковское окуругление DECIMAL
/// @param value Указатель на число для округления
/// @param right_bit Потерянный бит справа
void round_banking(s21_decimal* value, bool right_bit) {
    if (right_bit) {
        s21_decimal one = {{1, 0, 0, 0}};
        set_sign(&one, get_sign(*value));
        set_exponent(&one, get_exponent(*value));
        s21_add(*value, one, value);
    }
}
