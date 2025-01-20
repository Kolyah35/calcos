#pragma once
#include <platform.h>
#include <config.h>

#define LOW_BYTE(w) ((uint8_t) ((w) & 0xff))
#define HIGH_BYTE(w) ((uint8_t) ((w) >> 8))

#define BIT_READ(value, bit) (((value) >> (bit)) & 0x01)
#define BIT_SET(value, bit) ((value) |= (1UL << (bit)))
#define BIT_CLEAR(value, bit) ((value) &= ~(1UL << (bit)))
#define BIT_TOGGLE(value, bit) ((value) ^= (1UL << (bit)))
#define BIT_WRITE(value, bit, bitvalue) ((bitvalue) ? BIT_SET(value, bit) : BIT_CLEAR(value, bit))

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define CLAMP(VAL, MIN, MAX) VAL < MIN ? MIN : (VAL > MAX ? MAX : VAL)

#ifdef PLATFORM_AVR
    #define IS_PGM(ptr) (__builtin_constant_p(ptr) && (uintptr_t)(ptr) >= PROGMEM_START)
#else
    #define IS_PGM(ptr) (0)
#endif

#if CS_PIN < 8
    #define CS_IDLE     BIT_SET(PORTD, CS_PIN)
    #define CS_ACTIVE   BIT_CLEAR(PORTD, CS_PIN)
#elif CS_PIN < 14
    #define CS_IDLE     BIT_SET(PORTB, CS_PIN - 8)
    #define CS_ACTIVE   BIT_CLEAR(PORTB, CS_PIN - 8)
#elif CS_PIN < 20
    #define CS_IDLE     BIT_SET(PORTC, CS_PIN - 14)
    #define CS_ACTIVE   BIT_CLEAR(PORTC, CS_PIN - 14)
#endif

#if DC_PIN < 8
    #define DC_DATA      BIT_SET(PORTD, DC_PIN)
    #define DC_COMMAND   BIT_CLEAR(PORTD, DC_PIN)
#elif DC_PIN < 14
    #define DC_DATA      BIT_SET(PORTB, DC_PIN - 8)
    #define DC_COMMAND   BIT_CLEAR(PORTB, DC_PIN - 8)
#elif DC_PIN < 20
    #define DC_DATA      BIT_SET(PORTC, DC_PIN - 14)
    #define DC_COMMAND   BIT_CLEAR(PORTC, DC_PIN - 14)
#endif

#ifdef PLATFORM_AVR
    #include <util/delay.h>
    #define delay_ms(x) _delay_ms(x)
#else
    #include <raylib.h>
    #define delay_ms(x) WaitTime(x / 1000)
#endif
