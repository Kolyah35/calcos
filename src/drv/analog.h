#pragma once
#include <stdint.h>

#define DEFAULT 0

void analog_write(uint8_t pin, uint16_t duty);
uint16_t analog_read(uint8_t pin);
