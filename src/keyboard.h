#pragma once
#include <stdbool.h>
#include <config.h>

void init_keyboard();
void update_keyboard();
bool is_key_pressed(calc_key_t key);
calc_key_t get_pressed_key();