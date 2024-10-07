#include <display.h>
#include <config.h>

#ifdef PLATFORM_AVR

#include <utils.h>
#include <digital.h>
#include <spi.h>

//------------------------------
// ST7565 Commands
#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_DISP_START_LINE  0x40
#define CMD_SET_PAGE  0xB0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2 
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST  0x81
#define CMD_SET_VOLUME_SECOND  0
#define CMD_SET_STATIC_OFF  0xAC
#define CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define CMD_SET_BOOSTER_5  1
#define CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST  0xF0
//------------------------------

void init_display() {
    pin_mode(CS_PIN, OUTPUT);
    pin_mode(DC_PIN, OUTPUT);
    pin_mode(RST_PIN, OUTPUT);

    digital_write(RST_PIN, true);
    _delay_ms(500);
    digital_write(RST_PIN, false);

    spi_begin();

    CS_ACTIVE;
    DC_COMMAND;
    spi_transfer(CMD_INTERNAL_RESET);
    spi_transfer(CMD_DISPLAY_OFF);
    spi_transfer(CMD_SET_DISP_START_LINE);

    spi_transfer(CMD_SET_ADC_REVERSE);
    spi_transfer(CMD_SET_COM_NORMAL);

    spi_transfer(CMD_SET_DISP_NORMAL);
    spi_transfer(CMD_SET_BIAS_9);
    spi_transfer(CMD_SET_POWER_CONTROL | 0x7);
    
    spi_transfer(CMD_SET_BOOSTER_FIRST);
    spi_transfer(CMD_SET_BOOSTER_234);

    spi_transfer(CMD_SET_RESISTOR_RATIO | 0x5);

    // spi_transfer(0x81);
    // spi_transfer(170);

    spi_transfer(CMD_DISPLAY_OFF);
    spi_transfer(CMD_SET_ALLPTS_ON);
    CS_IDLE;
}

void set_display_contrast(uint8_t val) {
    SEND_CMD(CMD_SET_VOLUME_FIRST);
    SEND_CMD(val & 0x3f);
}

void clear_display() {
    for(uint16_t y = 0; y < SCREEN_HEIGHT / 8; y++) {
        for(uint16_t x = 0; x <= SCREEN_WIDTH; x++) {
            SEND_CMD(CMD_SET_COLUMN_UPPER | ((x >> 4)));
            SEND_CMD(CMD_SET_COLUMN_LOWER | (x & 0xf));
            SEND_DATA(0x0);
        }

        SEND_CMD(CMD_SET_PAGE | y);
    }
}

void set_display_pixel(uint16_t x, uint16_t y, color_t color) {
    SEND_CMD(CMD_SET_COLUMN_UPPER | (x >> 4));
    SEND_CMD(CMD_SET_COLUMN_LOWER | (x & 0xf));
    SEND_CMD(CMD_SET_PAGE | y / 8);
    SEND_DATA(0x1); // Временный вариант
}

#endif // PLATFORM_AVR