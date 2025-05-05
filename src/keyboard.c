#include <keyboard.h>
#include <config.h>
#include <platform.h>
#include <stdint.h>
#include <uart.h>
#include <drv/digital.h>
#include <debug.h>
#include <utils.h>
#include <string.h>

#ifdef PLATFORM_SIM
    #include <raylib.h>
#endif

////////////////////////////////////////
// KEYBOARD DRIVER UNDER CONSTRUCTION //
////////////////////////////////////////

calc_key_t pressed_keys[MAX_KEYS_PRESSED];
uint8_t pressed_keys_count = 0;

#if USE_MATRIX_KEYBOARD && !defined(PLATFORM_SIM)
uint8_t col_pins[MATRIX_KEYBOARD_COLS] = COL_PINS;
uint8_t row_pins[MATRIX_KEYBOARD_ROWS] = ROW_PINS;
volatile uint16_t key_states = 0;
volatile uint16_t prev_key_states = 0;
#endif

// temp debug feature
uint16_t get_key_states() {
#ifdef PLATFORM_SIM
    return 0;
#else
    return key_states;
#endif
}

void init_keyboard() {
#if defined(PLATFORM_AVR) && USE_MATRIX_KEYBOARD
    for (uint8_t r = 0; r < MATRIX_KEYBOARD_ROWS; r++) {
        pin_mode(row_pins[r], INPUT_PULLUP);
        digital_write(row_pins[r], 1);
    }

    for (uint8_t c = 0; c < MATRIX_KEYBOARD_COLS; c++) {
        pin_mode(col_pins[c], OUTPUT);
        digital_write(col_pins[c], 0);
    }
#endif
}

void update_keyboard() {
#ifdef PLATFORM_SIM
        pressed_keys[pressed_keys_count++] = GetKeyPressed();
#endif

#ifdef PLATFORM_AVR
    prev_key_states = key_states;
    key_states = 0;

    #if USE_SERIAL_CONTROL
        while (uart_available() && uart_read() == 'k' && pressed_keys_count < MAX_KEYS_PRESSED) {
            pressed_keys[pressed_keys_count++] = uart_read();
        }
    #endif

    #if USE_MATRIX_KEYBOARD
        for (uint8_t c = 0; c < MATRIX_KEYBOARD_COLS; c++) {
            digital_write(col_pins[c], 1);
            delay_ms(10);
            
            for (uint8_t r = 0; r < MATRIX_KEYBOARD_ROWS; r++) {
                if (!digital_read(row_pins[r])) {
                    BIT_SET(key_states, r * MATRIX_KEYBOARD_COLS + c);
                }
            }
            
            digital_write(col_pins[c], 0);
        }        
    #endif
#endif
}

bool is_key_pressed(calc_key_t key) {
    for(int i = 0; i < pressed_keys_count; i++) {
        if(pressed_keys[i] == key) {
            return true;
        }
    }

    return false;
}

calc_key_t get_pressed_key() {
    calc_key_t key = 0;

    if(pressed_keys_count > 0) {
        key = pressed_keys[0];

        for (int i = 0; i < pressed_keys_count - 1; i++) {
            pressed_keys[i] = pressed_keys[i + 1];
        }

        pressed_keys_count--;
    }

    return key;
}