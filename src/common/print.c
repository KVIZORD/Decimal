#include "services.h"


/// @brief Печать числа DECIMAL - битовое представление
/// @param value Число для печати
void print_decimal(s21_decimal value) {
    for (int i = INTS_IN_DECIMAL; i >= 0; i--) {
        print_binary_int(value.bits[i]);
        printf("|");
    }
    printf("\n");
}

// /// @brief 
// /// @param value 
// /// @param count_int 
// void print_arr_int(int* value, int count_int) {
//     for (int i = count_int - 1; i >= 0; i--) {
//         print_binary_int(value[i]);
//     }
//     printf("\n");
// }

/// @brief Печать числа INT - битовое представление
/// @param number Число для печати
void print_binary_int(int number) {
    for (int i = BITS_IN_INT - 1; i >= 0; i--) {
        printf("%d", ((unsigned int)number & (1 << i)) >> i);
    }
}

/// @brief Печать числа DECIMAL - десятичное представление
/// @param value Число для печати
void print_decimal_in_dec(s21_decimal value) {
    if (get_sign(value)) {
        printf("-");
    } else {
        printf("+");
    }
    char buf[30] = {0,};
    int i = 29;
    __int128_t val = decimal_to_int128(value);
    if (val == 0) {
        buf[i] = '0';
        i--;
    }
     else {
        for (; val > 0 && i > 0; i--, val /= 10) {
            buf[i] = "0123456789abcdef"[val % 10];
        }
    }
    for (; i > 0; i--) {
        buf[i] = ' ';
    }
    printf("%s", &(buf[i+1]));
    printf("*10^(-%u)\n", get_exponent(value));
}

/// @brief Конвертирование числа DECIMAL в __int128_t без учета знака числа
/// @param value Конверируемое число
/// @return Число преобразованное в __int128_t
__int128_t decimal_to_int128(s21_decimal value) {
    __int128_t cnt = 0;
    __int128_t factor = 1;
    for (int i = 0; i < INTS_IN_DECIMAL; i++) {
        for (int j = 0; j < BITS_IN_INT; j++) {
            cnt += factor * (__int128_t)get_bit_decimal(value, i, j);
            factor *= 2;
        }
    }
    return cnt;
}

