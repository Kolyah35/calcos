#include <config.h>
#include <string.h>

#if defined(PLATFORM_AVR) && USE_DISPLAY && defined(USE_DISPLAY_ST7565)

#include <utils.h>
#include <digital.h>
#include <spi.h>
#include <display.h>

#if DISPLAY_COLOR_DEPTH != 2
    #error Unsupported display color depth
#endif

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

uint8_t framebuffer[1024] = { 0 };

void display_init() {
    pin_mode(DISPLAY_CS_PIN, OUTPUT);
    pin_mode(DISPLAY_DC_PIN, OUTPUT);
    pin_mode(DISPLAY_RST_PIN, OUTPUT);

    spi_begin();
    SPI_SET_CLOCK_DIVIDER(SPI_CLOCK_DIV8);
    SPI_SET_DATA_MODE(SPI_MODE3);

    digital_write(DISPLAY_RST_PIN, false);
    _delay_ms(10);

    digital_write(DISPLAY_RST_PIN, true);
    digital_write(DISPLAY_CS_PIN, false);
    digital_write(DISPLAY_DC_PIN, false);

    spi_transfer(CMD_SET_BIAS_7);
    spi_transfer(CMD_SET_ADC_NORMAL);
    spi_transfer(CMD_SET_COM_NORMAL);
    spi_transfer(CMD_SET_DISP_START_LINE);
    spi_transfer(CMD_SET_POWER_CONTROL | 0x7);
    spi_transfer(CMD_SET_RESISTOR_RATIO | 0x6);
    spi_transfer(CMD_DISPLAY_ON);
    spi_transfer(CMD_SET_ALLPTS_NORMAL);
}

void display_set_contrast(uint8_t val) {
    SEND_CMD(CMD_SET_VOLUME_FIRST);
    SEND_CMD(CMD_SET_VOLUME_SECOND | (val & 0x3f));
}

void display_clear() {
    memset(&framebuffer, 0, sizeof(framebuffer));
    display_update();
}

void display_update() {
    for(uint16_t y = 0; y < 8; y++) {
        SEND_CMD(CMD_SET_PAGE | y);
        SEND_CMD(CMD_SET_COLUMN_UPPER | 0);
        SEND_CMD(CMD_SET_COLUMN_LOWER | 0);

        for(uint16_t x = 0; x < DISPLAY_WIDTH; x++) {
            SEND_DATA(framebuffer[(y * DISPLAY_WIDTH) + x]);
        }
    }
}

void display_set_pixel(uint16_t x, uint16_t y, uint8_t color) {
    if ((x < 0) || (x > DISPLAY_WIDTH) || (y < 0) || (y > DISPLAY_HEIGHT)) {
        return;
    }

    BIT_WRITE(framebuffer[(DISPLAY_WIDTH - x - 1) + (y / 8) * DISPLAY_WIDTH], (y & 7), color);
}

#endif // PLATFORM_AVR