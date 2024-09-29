#pragma once

#define LOW_BYTE(w) ((uint8_t) ((w) & 0xff))
#define HIGH_BYTE(w) ((uint8_t) ((w) >> 8))

#define BIT_READ(value, bit) (((value) >> (bit)) & 0x01)
#define BIT_SET(value, bit) ((value) |= (1UL << (bit)))
#define BIT_CLEAR(value, bit) ((value) &= ~(1UL << (bit)))
#define BIT_TOGGLE(value, bit) ((value) ^= (1UL << (bit)))
#define BIT_WRITE(value, bit, bitvalue) ((bitvalue) ? BIT_SET(value, bit) : BIT_CLEAR(value, bit))

// Default clock source is internal 16MHz RC oscillator
#define F_CPU 16000000UL

#include <util/delay.h>