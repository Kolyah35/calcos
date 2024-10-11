#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <config.h>
#include <font.h>
#include <utils.h>
#include <gfx.h>

ui_menu_t* load_ui_menu(const char* title, ui_menu_element* elements, int element_count) {
    ui_menu_t* menu = (ui_menu_t*)malloc(sizeof(ui_menu_t));
    menu->elements = elements;
    menu->element_count = element_count;
    menu->title = title;
    menu->node.width = measure_str_width(title) + 4;
    menu->node.height = element_count * MENU_ELEMENT_HEIGHT + GLYPH_HEIGHT + 8;
    menu->selected_element = 0;
    menu->node.draw = (node_callback_t)draw_ui_menu;
    menu->node.should_redraw = true;

    for(int i = 0; i < element_count; i++) {
        menu->node.width = MAX(menu->node.width, measure_str_width(elements[i].caption) + 4);
    }

    return menu;
}

void unload_ui_menu(ui_menu_t* menu) {
    free(menu->elements);
    free(menu);
}

void draw_ui_menu(ui_menu_t* menu, screen_t* parent) {
    unsigned int x = (SCREEN_WIDTH - DOCK_WIDTH) * 0.5 - menu->node.width * 0.5;
    unsigned int y = SCREEN_HEIGHT * 0.5 - menu->node.height * 0.5;

    draw_rectangle_filled(x, y, menu->node.width, menu->node.height, COLOR_BLACK);
    draw_rectangle(x, y, menu->node.width, menu->node.height, COLOR_WHITE);

    draw_text(menu->title, menu->node.width * 0.5 - measure_str_width(menu->title) * 0.5, 2, COLOR_WHITE);
    draw_line(x, GLYPH_HEIGHT + 2, menu->node.width, GLYPH_HEIGHT + 2, COLOR_WHITE);

    for(int i = 0; i < menu->element_count; i++) {
        if(i == menu->selected_element) { 
            draw_rectangle_filled(x + 2, (GLYPH_HEIGHT + 4) + (GLYPH_HEIGHT * i), menu->node.width - 4, GLYPH_HEIGHT + 2, COLOR_WHITE);
        }

        draw_text(menu->elements[i].caption, x + 3, (GLYPH_HEIGHT + 4) + (GLYPH_HEIGHT * i) + 1, (i == menu->selected_element ? COLOR_BLACK : COLOR_WHITE));
    }
}