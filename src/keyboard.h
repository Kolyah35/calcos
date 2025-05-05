#pragma once
#include <stdbool.h>
#include <config.h>
#include <stdint.h>

void init_keyboard();
void update_keyboard();
bool is_key_pressed(calc_key_t key);
calc_key_t get_pressed_key();

uint16_t get_key_states();