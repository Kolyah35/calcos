#include <display.h>
#include <digital.h>
#include <spi.h>
#include <config.h>

//------------------------------
// ST7567 Commands
#define ST7567_POWER_ON         0x2F // internal power supply on
#define ST7567_POWER_CTL        0x28 // internal power supply off
#define ST7567_CONTRAST         0x80 // 0x80 + (0..31)
#define ST7567_SEG_NORMAL       0xA0 // SEG remap normal
#define ST7567_SEG_REMAP        0xA1 // SEG remap reverse (flip horizontal)
#define ST7567_DISPLAY_NORMAL   0xA4 // display ram content
#define ST7567_DISPLAY_TEST     0xA5 // all pixels on
#define ST7567_INVERT_OFF       0xA6 // not inverted
#define ST7567_INVERT_ON        0xA7 // inverted
#define ST7567_DISPLAY_ON       0XAF // display on
#define ST7567_DISPLAY_OFF      0XAE // display off
#define ST7567_STATIC_OFF       0xAC
#define ST7567_STATIC_ON        0xAD
#define ST7567_SCAN_START_LINE  0x40 // scrolling 0x40 + (0..63)
#define ST7567_COM_NORMAL       0xC0 // COM remap normal
#define ST7567_COM_REMAP        0xC8 // COM remap reverse (flip vertical)
#define ST7567_SW_RESET         0xE2 // connect RST pin to GND and rely on software reset
#define ST7567_NOP              0xE3 // no operation
#define ST7567_TEST             0xF0

#define ST7567_COL_ADDR_H       0x10 // x pos (0..95) 4 MSB
#define ST7567_COL_ADDR_L       0x00 // x pos (0..95) 4 LSB
#define ST7567_PAGE_ADDR        0xB0 // y pos, 8.5 rows (0..8)
#define ST7567_RMW              0xE0
#define ST7567_RMW_CLEAR        0xEE

#define ST7567_BIAS_9           0xA2 
#define ST7567_BIAS_7           0xA3

#define ST7567_VOLUME_FIRST     0x81
#define ST7567_VOLUME_SECOND    0x00

#define ST7567_RESISTOR_RATIO   0x20
#define ST7567_STATIC_REG       0x0
#define ST7567_BOOSTER_FIRST    0xF8
#define ST7567_BOOSTER_234      0
#define ST7567_BOOSTER_5        1
#define ST7567_BOOSTER_6        3
//------------------------------

void init_display() {
    pin_mode(CS_PIN, OUTPUT);
    pin_mode(DC_PIN, OUTPUT);
    pin_mode(RST_PIN, OUTPUT);

    digital_write(RST_PIN, true);
    delay(50);
    digital_write(RST_PIN, false);

    spi_begin();
    SPI_SET_DATA_MODE(SPI_MODE0);
    SPI_SET_CLOCK_DIVIDER(SPI_CLOCK_DIV2);

    CS_ACTIVE;

    send_command(ST7567_BIAS_7);
    send_command(ST7567_SEG_NORMAL);
    send_command(ST7567_COM_REMAP);
    send_command(ST7567_POWER_CTL | 0x4);
    send_command(ST7567_POWER_CTL | 0x6);
    send_command(ST7567_POWER_CTL | 0x7);
    send_command(ST7567_RESISTOR_RATIO | 0x6);
    send_command(ST7567_SCAN_START_LINE);
    send_command(ST7567_DISPLAY_ON);
    send_command(ST7567_SEG_NORMAL);

    CS_IDLE;
}

void set_display_cursor(uint16_t x, uint16_t y) {
    CS_ACTIVE;
    send_command(ST7567_PAGE_ADDR  | y);
    send_command(ST7567_COL_ADDR_H | (x >> 4));
    send_command(ST7567_COL_ADDR_L | (x & 0xf));
    send_command(ST7567_RMW);
}

void set_display_contrast(uint8_t val) {
    CS_ACTIVE;
    send_command(ST7567_SCAN_START_LINE | (val & 0x3f));
    CS_IDLE;
}

void set_display_invert(bool val) {
    CS_ACTIVE;
    send_command(val ? ST7567_INVERT_ON : ST7567_INVERT_OFF);
    CS_IDLE;
}

void clear_display() {
    DC_DATA;
    for(uint16_t y = 0; y < SCREEN_HEIGHT; y++) {
        set_display_cursor(0, y);
        for(uint16_t x = 0; i < SCREEN_WIDTH; x++) {
            spi_transfer(0x00);
        }
    }
    CS_IDLE;
}

void draw_pixel(uint16_t x, uint16_t y, color_t color) {
    set_display_cursor(x, y);
    spi_transfer(0x01); // Надо порешать вопрос с цветом, потом разберемся 
}