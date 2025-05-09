#include "screen.h"
#include <stdlib.h>
#include <gfx.h>
#include <drv/display.h>
#include <home_screen.h>
#include <flasher_screen.h>
#include <stdio.h>
#include <keyboard.h>
#include <debug.h>
#include <imgui.h>

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

void draw_screen(screen_t* screen) {
    fill_rect(0, 0, display_width - DOCK_WIDTH, display_height, COLOR_BLACK);

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

void init_screen(screen_t* screen, uint8_t type, const char* option_top, const char* option_center, const char* option_bottom) {
    screen->type = type;
    screen->should_redraw = true;
}