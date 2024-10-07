#include <keyboard.h>
#include <config.h>
#include <platform.h>
#include <stdint.h>

#ifdef CALCOS_SIM
    #include <raylib.h>
#endif

key_t pressed_keys[MAX_KEYS_PRESSED];
uint8_t pressed_keys_count = 0;

void update_keyboard() {
    pressed_keys_count = 0;

    for(int i = 0; i < MAX_KEYS_PRESSED; i++) {
        key_t pressed_key = 0;

        switch(GetKeyPressed()) {
            case KEY_KP_0: pressed_key = BUTTON_ZERO; break;
            case KEY_KP_1: pressed_key = BUTTON_ONE; break;
            case KEY_KP_2: pressed_key = BUTTON_TWO; break;
            case KEY_KP_3: pressed_key = BUTTON_THREE; break;
            case KEY_KP_4: pressed_key = BUTTON_FOUR; break;
            case KEY_KP_5: pressed_key = BUTTON_FIVE; break;
            case KEY_KP_6: pressed_key = BUTTON_SIX; break;
            case KEY_KP_7: pressed_key = BUTTON_SEVEN; break;
            case KEY_KP_8: pressed_key = BUTTON_EIGHT; break;
            case KEY_KP_9: pressed_key = BUTTON_NINE; break;

            case KEY_END: pressed_key = BUTTON_POWER; break;
            case KEY_BACKSPACE: pressed_key = BUTTON_CE; break;
            case KEY_P: pressed_key = BUTTON_PERCENT; break;
            case KEY_S: pressed_key = BUTTON_SQRT; break;
            case KEY_SLASH: pressed_key = BUTTON_DIVIDE; break;
            case KEY_PAGE_UP: pressed_key = BUTTON_M_PLUS; break;
            case KEY_PAGE_DOWN: pressed_key = BUTTON_M_MINUS; break;
            case KEY_HOME: pressed_key = BUTTON_MRC; break;
            case KEY_KP_MULTIPLY: pressed_key = BUTTON_MULTIPLY; break;
            case KEY_KP_SUBTRACT: pressed_key = BUTTON_MINUS; break;
            case KEY_KP_ADD: pressed_key = BUTTON_PLUS; break;
            case KEY_KP_EQUAL: pressed_key = BUTTON_EQUAL; break;
            case KEY_COMMA: pressed_key = BUTTON_DOT; break;
        }

        pressed_keys[i] = pressed_key;
        pressed_keys_count++;
    }
}

bool is_key_pressed(key_t key) {
    for(int i = 0; i < pressed_keys_count; i++) {
        if(pressed_keys[i] == key) {
            return true;
        }
    }

    return false;
}

key_t get_pressed_key() {
    key_t key = 0;

    if(pressed_keys_count > 0) {
        key = pressed_keys[0];

        for (int i = 0; i < pressed_keys_count - 1; i++) {
            pressed_keys[i] = pressed_keys[i + 1];
        }

        pressed_keys_count--;
    }

    return key;
}