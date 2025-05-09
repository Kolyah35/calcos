#pragma once
#include <stdbool.h>
#include <config.h>
#include <stdint.h>

void init_keyboard();
void update_keyboard();
bool is_key_pressed(calc_key_t key);
bool is_key_down(calc_key_t key);