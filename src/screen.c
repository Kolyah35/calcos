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

int add_node_to_screen(screen_t* screen, ui_node_t* node) {
    if(screen != NULL) {
        if(screen->node_count > 0) {
            screen->ui_nodes = realloc(screen->ui_nodes, (screen->node_count + 1) * sizeof(void*));
        } else {
            screen->ui_nodes = malloc(sizeof(void*));
        }

        screen->ui_nodes[screen->node_count] = node;
        screen->node_count++;

        return screen->node_count - 1; // return index
    }

    return -1;
}

void delete_node_from_screen(int index, bool free_node) {
    if(screen != NULL && index < screen->node_count) {
        if(free_node) {
            screen->ui_nodes[index]->unload(screen->ui_nodes[index]);
        }

        for(int i = index; i < screen->node_count; i++) {
            screen->ui_nodes[i] = screen->ui_nodes[i + 1];
        }

        screen->node_count--;
        screen->ui_nodes = realloc(screen->ui_nodes, screen->node_count * sizeof(void*));
    }
}