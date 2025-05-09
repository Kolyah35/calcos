#include <keyboard.h>
#include <config.h>
#include <platform.h>

////////////////////////////////////////
// KEYBOARD DRIVER UNDER CONSTRUCTION //
////////////////////////////////////////

#ifdef PLATFORM_AVR

#include <stdint.h>
#include <uart.h>
#include <drv/digital.h>
#include <debug.h>
#include <utils.h>


#if USE_MATRIX_KEYBOARD
uint8_t col_pins[MATRIX_KEYBOARD_COLS] = COL_PINS;
uint8_t row_pins[MATRIX_KEYBOARD_ROWS] = ROW_PINS;
uint8_t prev_key_states[(MATRIX_KEYBOARD_COLS * MATRIX_KEYBOARD_ROWS) / 8];
uint8_t key_states[(MATRIX_KEYBOARD_COLS * MATRIX_KEYBOARD_ROWS) / 8];
#endif

void init_keyboard() {
#if USE_MATRIX_KEYBOARD
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
    memcpy(prev_keystates, key_states, sizeof(key_states));
    memset(key_states, 0, sizeof(key_states));

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
                    int key = r * MATRIX_KEYBOARD_COLS + c;
                    BIT_SET(key_states[key / 8], key % 8);
                }
            }
            
            digital_write(col_pins[c], 0);
        }        
    #endif
}

bool is_key_pressed(calc_key_t key) {
    return BIT_READ(prev_key_states[key / 8], key % 8) && !(BIT_READ(key_states[key / 8], key % 8));
}

bool is_key_down(calc_key_t key) {
    return BIT_READ(prev_key_states[key / 8], key % 8) && BIT_READ(key_states[key / 8], key % 8);
}
#endif