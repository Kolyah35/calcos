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
    #define IS_PGM(ptr) (__builtin_constant_p(ptr) && (uintptr_t)(ptr) < 0x0100)
#else
    #define IS_PGM(ptr) (0)
#endif

#if DISPLAY_CS_PIN < 8
    #define DISPLAY_CS_IDLE     BIT_SET(PORTD, DISPLAY_CS_PIN)
    #define DISPLAY_CS_ACTIVE   BIT_CLEAR(PORTD, DISPLAY_CS_PIN)
#elif DISPLAY_CS_PIN < 14
    #define DISPLAY_CS_IDLE     BIT_SET(PORTB, DISPLAY_CS_PIN - 8)
    #define DISPLAY_CS_ACTIVE   BIT_CLEAR(PORTB, DISPLAY_CS_PIN - 8)
#elif DISPLAY_CS_PIN < 20
    #define DISPLAY_CS_IDLE     BIT_SET(PORTC, DISPLAY_CS_PIN - 14)
    #define DISPLAY_CS_ACTIVE   BIT_CLEAR(PORTC, DISPLAY_CS_PIN - 14)
#endif

#if DISPLAY_DC_PIN < 8
    #define DISPLAY_DC_DATA      BIT_SET(PORTD, DISPLAY_DC_PIN)
    #define DISPLAY_DC_COMMAND   BIT_CLEAR(PORTD, DISPLAY_DC_PIN)
#elif DISPLAY_DC_PIN < 14
    #define DISPLAY_DC_DATA      BIT_SET(PORTB, DISPLAY_DC_PIN - 8)
    #define DISPLAY_DC_COMMAND   BIT_CLEAR(PORTB, DISPLAY_DC_PIN - 8)
#elif DISPLAY_DC_PIN < 20
    #define DISPLAY_DC_DATA      BIT_SET(PORTC, DISPLAY_DC_PIN - 14)
    #define DISPLAY_DC_COMMAND   BIT_CLEAR(PORTC, DISPLAY_DC_PIN - 14)
#endif

#ifdef PLATFORM_AVR
    #include <util/delay.h>
    #define delay_ms(x) _delay_ms(x)
#else
    #include <raylib.h>
    #define delay_ms(x) WaitTime(x / 1000)
#endif
