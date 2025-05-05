#include <config.h>
#include <string.h>

#if defined(PLATFORM_AVR) && USE_DISPLAY && defined(USE_DISPLAY_ST7789)

#include <utils.h>
#include <digital.h>
#include <spi.h>
#include <display.h>
#include <debug.h>

#if DISPLAY_COLOR_DEPTH != 16
    #error Unsupported display color depth
#endif

#if DISPLAY_WIDTH == 240 && DISPLAY_HEIGHT == 240
  #define X_OFFSET 0
  #define Y_OFFSET 80

#elif DISPLAY_WIDTH == 135 && DISPLAY_HEIGHT == 240
  #define X_OFFSET 52
  #define Y_OFFSET 40

#elif DISPLAY_WIDTH == 170 && DISPLAY_HEIGHT == 320
  #define X_OFFSET 35
  #define Y_OFFSET 0

#else
  #define X_OFFSET 0
  #define Y_OFFSET 0
#endif

#if DISPLAY_ROTATION == 0
  #define X_START X_OFFSET
  #define Y_START Y_OFFSET

#elif DISPLAY_ROTATION == 1
  #define X_START Y_OFFSET
  #define Y_START X_OFFSET

#elif DISPLAY_ROTATION == 2
  #define X_START X_OFFSET
  #define Y_START Y_OFFSET

#elif DISPLAY_ROTATION == 3
  #define X_START Y_OFFSET
  #define Y_START X_OFFSET
#endif


// -----------------------------------------
// ST7789 commands
#define ST7789_SWRESET 0x01

#define ST7789_SLPIN   0x10  // sleep on
#define ST7789_SLPOUT  0x11  // sleep off
#define ST7789_PTLON   0x12  // partial on
#define ST7789_NORON   0x13  // partial off
#define ST7789_INVOFF  0x20  // invert off
#define ST7789_INVON   0x21  // invert on
#define ST7789_DISPOFF 0x28  // display off
#define ST7789_DISPON  0x29  // display on
#define ST7789_IDMOFF  0x38  // idle off
#define ST7789_IDMON   0x39  // idle on

#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

#define ST7789_PTLAR    0x30   // partial start/end
#define ST7789_VSCRDEF  0x33   // SETSCROLLAREA
#define ST7789_VSCRSADD 0x37

#define ST7789_WRDISBV  0x51
#define ST7789_WRCTRLD  0x53
#define ST7789_WRCACE   0x55
#define ST7789_WRCABCMB 0x5e

#define ST7789_POWSAVE    0xbc
#define ST7789_DLPOFFSAVE 0xbd

// bits in MADCTL
#define ST7789_MADCTL_MY  0x80
#define ST7789_MADCTL_MX  0x40
#define ST7789_MADCTL_MV  0x20
#define ST7789_MADCTL_ML  0x10
#define ST7789_MADCTL_RGB 0x00
//------------------------------

#if DISPLAY_WIDTH == 240 && DISPLAY_HEIGHT == 280
    #define START_X 0
    #define END_X 0
    #define START_Y 20
    #define END_Y 20
#elif DISPLAY_WIDTH == 240 && DISPLAY_HEIGHT == 240
    #define START_X 0
    #define END_X 0
    #define START_Y 80
    #define END_Y 0
#elif DISPLAY_WIDTH == 170 && DISPLAY_HEIGHT == 320
    #define START_X 35
    #define END_X 35
    #define START_Y 0
    #define END_Y 0
#else
    #define START_X 0
    #define END_X 0
    #define START_Y 0
    #define END_Y 0
#endif

int offset_x, offset_y = 0;
uint16_t display_width = DISPLAY_WIDTH;
uint16_t display_height = DISPLAY_HEIGHT;

static inline void write_color(uint16_t color, int count) {
    while (count--) spi_transfer16(color);
}

static inline void write_colors(uint16_t* colors, int count) {
    spi_transfer_n(colors, count * sizeof(uint16_t));
}

void display_init() {
    dbg_info("Adafruit ST7789 (%dx%d) display", display_width, display_height);

    offset_x = START_X;
    offset_y = START_Y;

    pin_mode(DISPLAY_CS_PIN, OUTPUT);
    pin_mode(DISPLAY_DC_PIN, OUTPUT);
    pin_mode(DISPLAY_RST_PIN, OUTPUT);

    spi_begin();
    SPI_SET_CLOCK_DIVIDER(SPI_CLOCK_DIV2);
    SPI_SET_DATA_MODE(SPI_MODE3);

    digital_write(DISPLAY_RST_PIN, true);
    _delay_ms(50);
    digital_write(DISPLAY_RST_PIN, false);
    _delay_ms(50);
    digital_write(DISPLAY_RST_PIN, true);
    _delay_ms(50);

    SEND_CMD(ST7789_SWRESET);
    _delay_ms(120);
    SEND_CMD(ST7789_SLPOUT);
    _delay_ms(120);
    
    SEND_CMD(ST7789_COLMOD); // 16 bit color
    SEND_DATA(0x55);

    display_set_rotation(1);

    SEND_CMD(ST7789_CASET);
    DISPLAY_DC_DATA;
    spi_transfer16(offset_x);
    spi_transfer16(display_width + offset_x);

    SEND_CMD(ST7789_RASET);
    DISPLAY_DC_DATA;
    spi_transfer16(offset_y);
    spi_transfer16(display_height + offset_y);

    SEND_CMD(ST7789_INVON);
    SEND_CMD(ST7789_DISPON);
    _delay_ms(20);

    // DISPLAY_CS_IDLE;
}

void display_set_cursor(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    x1 += offset_x, x0 += offset_x;
    y1 += offset_y, y0 += offset_y;

    SEND_CMD(ST7789_CASET);
    DISPLAY_DC_DATA;
    spi_transfer16(x0);
    spi_transfer16(x1);

    SEND_CMD(ST7789_RASET);
    DISPLAY_DC_DATA;
    spi_transfer16(y0);
    spi_transfer16(y1);

    SEND_CMD(ST7789_RAMWR);
}

void display_set_contrast(uint8_t val) {
    dbg_warn("ST7789 does not support contrast!");
}

void display_clear() {
    display_draw_rect_filled(0, 0, display_width, display_height, 0x0000);
}

void display_update() {
    // nothing to update here...
}

void display_set_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if(x < 0 || y < 0 || x >= display_width || y >= display_height) return;
    
    display_set_cursor(x, y, x, y);

    DISPLAY_DC_DATA;
    spi_transfer16(color);
    DISPLAY_CS_IDLE;
}

void display_set_rotation(uint8_t r) {
    switch (r & 3) {
        case 0:
            r = ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB;
            offset_x = START_X;
            offset_y = START_Y;
            display_width = DISPLAY_WIDTH;
            display_height = DISPLAY_HEIGHT;
            break;
        case 1:
            r = ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
            offset_x = START_Y;
            offset_y = START_X;
            display_width = DISPLAY_HEIGHT;
            display_height = DISPLAY_WIDTH;
            break;
       case 2:
            r = ST7789_MADCTL_RGB;
            offset_x = END_X;
            offset_y = END_Y;
            display_width = DISPLAY_WIDTH;
            display_height = DISPLAY_HEIGHT;
            break;
        case 3:
            r = ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
            offset_x = END_Y;
            offset_y = END_X;
            display_width = DISPLAY_HEIGHT;
            display_height = DISPLAY_WIDTH;
            break;
    }

    SEND_CMD(ST7789_MADCTL);
    SEND_DATA(r);
}

void display_draw_rect_filled(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (x < 0 || y < 0 || x >= display_width || y >= display_height || w <= 0 || h <= 0) return;
    // if (x + w > width)  w = width - x;
    // if (y + h > height) h = height - y;
    // if (x < 0) { w += x; x = 0; }
    // if (w <= 0) return;
    // if (y < 0) { h += y; y = 0; }
    // if (h <= 0) return;

    display_set_cursor(x, y, x + w - 1, y + h - 1);
    
    DISPLAY_DC_DATA;

    if((long)w * h > 0x10000) {
        write_color(color, 0);
    }

    write_color(color, w * h);

    DISPLAY_CS_IDLE;
}

void display_draw_line_vertical(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if(x < 0 || y < 0 || x >= display_width || y >= display_height || h <= 0) return;
    
    // if(y + h > height) {
    //     h = height - y;
    // }
    
    // if(y < 0) { 
    //     h += y; 
    //     y = 0;  
    // }
    
    // if(h <= 0) return;
    
    display_set_cursor(x, y, x, y + h - 1);
    
    DISPLAY_DC_DATA;
    
    write_color(color, h);

    DISPLAY_CS_IDLE;
}

void display_draw_line_horizontal(int16_t x, int16_t y, int16_t w,  uint16_t color) {
    if(x < 0 || y < 0 || x >= display_width || y >= display_height || w <= 0) return;
    
    // if(x + w > width) {
    //     w = width - x;
    // }

    // if(x < 0) { 
    //     w += x; 
    //     x = 0; 
    // }
    
    // if(w <= 0) return;
    display_set_cursor(x, y, x + w - 1, y);

    DISPLAY_DC_DATA;

    write_color(color, w);

    DISPLAY_CS_IDLE;
}

#endif // PLATFORM_AVR