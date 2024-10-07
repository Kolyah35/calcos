#include <home_screen.h>
#include <gfx.h>
#include <stdlib.h>

home_screen_t* home_screen = NULL;

home_screen_t* load_home_screen() {
    if(!home_screen) {
        home_screen = (home_screen_t*)malloc(sizeof(home_screen_t));
    }

    home_screen->screen.unload_callback = unload_home_screen;
    home_screen->screen.update_callback = home_screen_update;
    home_screen->screen.draw_callback = home_screen_draw;
    home_screen->screen.key_callback = home_screen_on_key;

    home_screen->screen.options[OPTION_TOP] = NULL;
    home_screen->screen.options[OPTION_CENTER] = NULL;
    home_screen->screen.options[OPTION_BOTTOM] = "Menu";

    return home_screen;
}

void unload_home_screen() {
    free(home_screen);
}

void home_screen_update() {}

void home_screen_draw() {
    // draw_text("ПРИВЕТ МИР!!", 1, 1, COLOR_WHITE);    
}

void home_screen_on_key(key_t key) {

}