#include <screen.h>
#include <stdlib.h>

screen_t* screen = 0;

void push_screen(screen_t* _screen) {
    free(screen);
    screen = _screen;
}

screen_t* get_current_screen() {
    return screen;
}

option_t key_to_option(key_t key) {
    switch(key) {
        case BUTTON_MULTIPLY: return OPTION_TOP;
        case BUTTON_MINUS: return OPTION_CENTER;
        case BUTTON_PLUS: return OPTION_BOTTOM;
        default: return OPTION_NONE;
    }
}

eicon_t opt_to_icon(option_t opt) {
    switch (opt) {
        case OPTION_TOP: return ICON_BTN_MULTIPLY;
        case OPTION_CENTER: return ICON_BTN_MINUS;
        case OPTION_BOTTOM: return ICON_BTN_PLUS;
        default: return ICON_NONE;
    }
}