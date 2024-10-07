#pragma once
#include <platform.h>
#include <types.h>

#ifdef PLATFORM_AVR
    #include <avr/io.h>
    #include <spi.h>
#endif

#define CS_IDLE     PORTB |= (1 << 2)
#define CS_ACTIVE   PORTB &= ~(1 << 2)
#define DC_DATA     PORTB |= (1 << 1)
#define DC_COMMAND  PORTB &= ~(1 << 1)

#ifdef PLATFORM_AVR
#define SEND_CMD(cmd) \
    DC_COMMAND; \
    spi_transfer(cmd);

#define SEND_DATA(data) \
    DC_DATA; \
    spi_transfer(data);
#endif

void init_display();
void set_display_contrast(uint8_t);
void clear_display();
void set_display_pixel(uint16_t x, uint16_t y, color_t color);