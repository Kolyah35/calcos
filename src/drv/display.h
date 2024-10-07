#pragma once
#include <platform.h>
#include <types.h>

#ifdef CALCOS_AVR
    #include <avr/io.h>
    #include <spi.h>
#endif

#define CS_PIN 10
#define DC_PIN 9
#define RST_PIN 9

#define CS_IDLE     PORTB |= (1 << 2)
#define CS_ACTIVE   PORTB &= ~(1 << 2)
#define DC_DATA     PORTB |= (1 << 1)
#define DC_COMMAND  PORTB &= ~(1 << 1)

#ifdef CALCOS_AVR
inline void send_command(uint8_t cmd) {
    DC_COMMAND;
    spi_transfer(cmd);
}

inline void send_data(uint8_t data) {
    DC_DATA;
    spi_transfer(data);
}
#endif

void init_display();
void set_display_cursor(uint16_t x, uint16_t y);
void set_display_contrast(uint8_t);
void set_display_invert(bool);
void clear_display();
void draw_pixel(uint16_t x, uint16_t y, color_t color);