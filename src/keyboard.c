#include <keyboard.h>
#include <config.h>
#include <platform.h>
#include <stdint.h>
#include <uart.h>

#ifdef PLATFORM_SIM
    #include <raylib.h>
#endif

calc_key_t pressed_keys[MAX_KEYS_PRESSED];
uint8_t pressed_keys_count = 0;

#if USE_MATRIX_KEYBOARD 
uint8_t col_pins[MATRIX_KEYBOARD_COLS] = COL_PINS;
uint8_t row_pins[MATRIX_KEYBOARD_ROWS] = ROW_PINS;
#endif

void init_keyboard() {
#if PLATFORM_AVR && USE_MATRIX_KEYBOARD
    for (byte r = 0; r < MATRIX_KEYBOARD_ROWS; r++) {
        pin_mode(row_pins[r], INPUT_PULLUP);
    }

    for (byte c = 0; c < MATRIX_KEYBOARD_COLS; c++) {
        digital_write(col_pins[c], HIGH);
        pin_mode(col_pins[c], INPUT);
    }
#endif
}

void update_keyboard() {
    pressed_keys_count = 0;

    for(int i = 0; i < MAX_KEYS_PRESSED; i++) {
        calc_key_t hardware_key = 0;

#ifdef PLATFORM_SIM
        hardware_key = GetKeyPressed();
#endif

#ifdef PLATFORM_AVR
    #if USE_SERIAL_CONTROL
        hardware_key = (uart_available() ? uart_read() : 0);
    #endif

    #if USE_MATRIX_KEYBOARD
        for (uint8_t c = 0; c < MATRIX_KEYBOARD_COLS; c++) {
            pin_mode(col_pins[c], OUTPUT);
            digital_write(col_pins[c], LOW);
            
            for (byte r = 0; r < MATRIX_KEYBOARD_ROWS; r++) {
                if (!digitalRead(row_pins[r]) && pressed_keys_count < MAX_KEYS_PRESSED) {
                    pressed_keys[pressed_keys_count] = r * MATRIX_KEYBOARD_COLS + c;
                    continue;
                }

                return;
            }

            digitalWrite(col_pins[c], HIGH);
            pinMode(col_pins[c], INPUT);
        }               
    #endif
#endif

        pressed_keys[i] = hardware_key;
        pressed_keys_count++;
    }
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