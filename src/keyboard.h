#pragma once
#include <stdbool.h>

typedef enum key_t {
    BUTTON_POWER = 1, 
    BUTTON_CE, 
    BUTTON_PERCENT, 
    BUTTON_SQRT,
    BUTTON_DIVIDE,
    BUTTON_M_PLUS,
    BUTTON_M_MINUS,
    BUTTON_MRC,  
    BUTTON_SEVEN,
    BUTTON_EIGHT,
    BUTTON_NINE,
    BUTTON_MULTIPLY,
    BUTTON_MINUS,
    BUTTON_SIX,
    BUTTON_FIVE,
    BUTTON_FOUR,
    BUTTON_ONE,
    BUTTON_TWO,
    BUTTON_THREE,
    BUTTON_PLUS,
    BUTTON_EQUAL,
    BUTTON_DOT,
    BUTTON_ZERO
} key_t;

void update_keyboard();
bool is_key_pressed(key_t key);
key_t get_pressed_key();