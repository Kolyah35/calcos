#include "screen.h"
#include <stdlib.h>
#include <gfx.h>
#include <ui/text.h>
#include <ui/menu.h>
#include <ui/image.h>
#include <home_screen.h>
#include <stdio.h>

screen_t* screen = NULL;
rect_t draw_area;

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
    if(screen->locked && node->type == UI_MENU) {
        return -1;
    }

    if(screen != NULL) {
        if(screen->node_count > 1) {
            screen->ui_nodes = realloc(screen->ui_nodes, (screen->node_count + 1) * sizeof(void*));
        }

        screen->ui_nodes[screen->node_count] = node;
        screen->node_count++;
        screen->should_redraw = true;

        if(node->type == UI_MENU) {
            screen->locked = true;
        }

        return screen->node_count - 1; // return index
    }

    return -1;
}

void delete_node_from_screen_i(int index, bool free_node) {
    if(screen != NULL && index < screen->node_count) {
        if(screen->ui_nodes[index]->type == UI_MENU) {
            screen->locked = false;
        }
        
        if(free_node) {
            unload_node(screen->ui_nodes[index]);
        }

        for(int i = index; i < screen->node_count; i++) {
            screen->ui_nodes[i] = screen->ui_nodes[i + 1];
        }

        screen->node_count--;
        // screen->ui_nodes = realloc(screen->ui_nodes, screen->node_count * sizeof(void*));
        screen->should_redraw = true;
    }
}

void delete_node_from_screen_ptr(ui_node_t* ptr, bool free_node) {
    if(screen != NULL && ptr != NULL) {
        for(int i = 0; i < screen->node_count; i++) {
            if(screen->ui_nodes[i] == ptr) {
                if(ptr->type == UI_MENU) {
                    screen->locked = false;
                }

                if(free_node) {
                    unload_node(ptr);
                }

                screen->node_count--;

                for(int j = i; j < screen->node_count; j++) {
                    screen->ui_nodes[j] = screen->ui_nodes[j + 1];
                }

                // screen->ui_nodes = realloc(screen->ui_nodes, screen->node_count * sizeof(void*));
                screen->should_redraw = true;

                return;
            }
        }
    }
}

void update_screen(screen_t* screen) {
    if(!screen->locked) {
        switch(screen->type) {
            case SCREEN_HOME: 
                update_home_screen();     
                break;
        }
    }

    for(int i = 0; i < screen->node_count; i++) {
        ui_node_t* node = screen->ui_nodes[i];

        switch(node->type) {
            case UI_MENU:
                update_ui_menu((ui_menu_t*)node);
                break;
            default:
                break;
        }

        if(node->should_redraw) {
            screen->should_redraw = true;
            node->should_redraw = false;
        }
    }
}


void draw_screen(screen_t* screen) {
    draw_rectangle_filled(0, 0, draw_area.w, draw_area.h, COLOR_WHITE);

    for(int i = 0; i < screen->node_count; i++) {
        ui_node_t* node = screen->ui_nodes[i];

        switch(node->type) {
            case UI_TEXT_MIN: 
                draw_ui_text_min((ui_text_min_t*)node); 
                break;
            
            case UI_MENU:
                draw_ui_menu((ui_menu_t*)node); 
                break;

            case UI_IMAGE:
                draw_ui_image((ui_image_t*)node); 
                break;

            default:
                break;
        }
    }
}

void unload_screen(screen_t* screen) {
    for(int i = 0; i < screen->node_count; i++) {
        unload_node(screen->ui_nodes[i]);
    }

    switch(screen->type) {
        case SCREEN_HOME: 
            unload_home_screen();
            break;
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