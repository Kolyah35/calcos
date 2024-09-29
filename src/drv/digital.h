#pragma once
#include <stdint.h>
#include <stdbool.h>

#define LED_BUILTIN 13

typedef enum pin_mode_e {
    INPUT = 0x0,
    OUTPUT = 0x1,
    INPUT_PULLUP = 0x2
} pin_mode_e;

void pin_mode(uint8_t pin, pin_mode_e mode);
void digital_write(uint8_t pin, bool val);
void digital_toggle(uint8_t pin);
bool digital_read(uint8_t pin);