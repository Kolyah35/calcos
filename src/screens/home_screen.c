#include <home_screen.h>
#include <gfx.h>
#include <stdlib.h>
#include <stdio.h>

home_screen_t* home_screen = NULL;

home_screen_t* load_home_screen() {
    if(!home_screen) {
        home_screen = (home_screen_t*)malloc(sizeof(home_screen_t));
    }

    home_screen->screen.unload_callback = unload_home_screen;
    home_screen->screen.update_callback = home_screen_update;
    // home_screen->screen.draw_callback = home_screen_draw;

    home_screen->screen.options[OPTION_TOP] = "Test";
    home_screen->screen.options[OPTION_CENTER] = "Test";
    home_screen->screen.options[OPTION_BOTTOM] = "Menu";

    return home_screen;
}

void unload_home_screen() {
    for(int i = 0; i < home_screen->screen.node_count; i++) {
        home_screen->screen.ui_nodes[i]->unload(home_screen->screen.ui_nodes[i]);
    }

    free(home_screen);
}

void home_screen_update() {}

void home_screen_draw() {
    draw_rectangle_filled(0, 0, 128, 64, COLOR_WHITE);
    char str[20];
    sprintf(str, "Prssd key: %02x", get_pressed_key());

    draw_text(str, 1, 1, COLOR_BLACK);
}