#include "screen.h"
#include <stdlib.h>
#include <gfx.h>
#include <home_screen.h>
#include <flasher_screen.h>
#include <stdio.h>
#include <keyboard.h>
#include <debug.h>
#include <imgui.h>

// screen_t* screen = NULL;
rect_t draw_area;

void push_screen(screen_t* _screen) {
    if(screen != NULL) {
        unload_screen(screen);
    }

    screen = _screen;
    screen->should_redraw = true;
}

screen_t* get_current_screen() {
    return screen;
}

option_t key_to_option(int key) {
    switch(key) {
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

void draw_screen(screen_t* screen) {
    draw_rectangle_filled(0, 0, draw_area.w, draw_area.h, COLOR_WHITE);

    switch (screen->type) {
        case SCREEN_HOME: return draw_home_screen((home_screen_t*)screen);
        case SCREEN_FLASHER: return draw_flasher_screen((flasher_screen_t*)screen);
    }
}

void update_screen(screen_t* screen) {
    imgui_update_ctx();
}

void unload_screen(screen_t* screen) {
    switch(screen->type) {
        case SCREEN_FLASHER: 
            unload_flasher_screen((flasher_screen_t*)screen);
            break;
        default:
            free(screen);
            screen = NULL;
    }
}

bool is_keyboard_locked() {
    return screen->locked;
}

void lock_keyboard() {
    screen->locked = true;
}

void unlock_keyboard() {
    screen->locked = false;
}

void init_screen(screen_t* screen, uint8_t type, const char* option_top, const char* option_center, const char* option_bottom) {
    screen->type = type;
    screen->locked = false;
    screen->options[OPTION_TOP] = option_top;
    screen->options[OPTION_CENTER] = option_center;
    screen->options[OPTION_BOTTOM] = option_bottom;
}