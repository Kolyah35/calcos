#include <home_screen.h>
#include <gfx.h>
#include <stdlib.h>
#include <stdio.h>
#include <text.h>

home_screen_t* home_screen = NULL;

home_screen_t* load_home_screen() {
    if(!home_screen) {
        home_screen = (home_screen_t*)malloc(sizeof(home_screen_t));
    }

    home_screen->screen.node_count = 0;

    home_screen->screen.unload_callback = unload_home_screen;
    home_screen->screen.update_callback = home_screen_update;
    // home_screen->screen.draw_callback = home_screen_draw;

    home_screen->screen.options[OPTION_TOP] = NULL;
    home_screen->screen.options[OPTION_CENTER] = NULL;
    home_screen->screen.options[OPTION_BOTTOM] = "Menu";

    ui_text_min_t* text_node = load_ui_text_min(DISPLAY_BLACK, NULL);
    text_node->x = 1;
    text_node->y = 1;

    add_node_to_screen(home_screen, text_node);

    return home_screen;
}

void unload_home_screen() {
    for(int i = 0; i < home_screen->screen.node_count; i++) {
        home_screen->screen.ui_nodes[i]->unload(home_screen->screen.ui_nodes[i]);
    }

    free(home_screen);
}

void home_screen_update() {
    char str[20];
    sprintf(str, "Prssd key: %02x", get_pressed_key());

    ui_text_min_t* text_node = (ui_text_min_t*)home_screen->screen.ui_nodes[0];

    text_node->text = str;

    home_screen->screen.should_redraw = true;
}