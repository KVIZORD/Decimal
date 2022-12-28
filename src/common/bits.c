#include "services.h"


// DECIMAL
int get_bit_decimal(s21_decimal value, int num_int, int ind) { 
    return (value.bits[num_int] >> ind) & 1;
}

int set_bit_decimal(s21_decimal* value, int num_int, int ind) {    
    value->bits[num_int] |= 1 << ind;
    return true;
}

int reset_bit_decimal(s21_decimal* value, int num_int, int ind) {
    value->bits[num_int] &= ~(1 << ind);
    return true;
}

// DOUBLE DECIMAL
int get_bit_double_decimal(s21_double_decimal value, int num_int, int ind) { 
    return (value.bits[num_int] >> ind) & 1;
}

int set_bit_double_decimal(s21_double_decimal* value, int num_int, int ind) {    
    value->bits[num_int] |= 1 << ind;
    return true;
}

int reset_bit_double_decimal(s21_double_decimal* value, int num_int, int ind) {
    value->bits[num_int] &= ~(1 << ind);
    return true;
}

// INT
int get_bit_int(int number, int ind) { 
    return number & (1 << ind);
}

int set_bit_int(int* number, int ind) {  
    *number |= 1 << ind;
    return true;
}

int reset_bit_int(int* number, int ind) {  
    *number &= ~(1 << ind);
    return true;
}

// INTS
int get_bit_arr_int(int* value, int num_int, int ind) { 
    return get_bit_int(value[num_int], ind);
}

int set_bit_arr_int(int* value, int num_int, int ind) {
    set_bit_int(&(value[num_int]), ind);
    return true;
}

int reset_bit_arr_int(int* value, int num_int, int ind) {
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

// 1.3292279165568E+36