#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <config.h>
#include <font.h>
#include <utils.h>
#include <gfx.h>
#include <keyboard.h>

ui_menu_t* load_ui_menu(const char* title, ui_menu_element* elements, int element_count) {
    ui_menu_t* menu = (ui_menu_t*)malloc(sizeof(ui_menu_t));
    menu->title = title;
    menu->elements = elements;
    menu->element_count = element_count;
    menu->selected_element = 0;
    menu->node.type = UI_MENU;
    menu->node.width = measure_str_width(title) + 4;
    menu->node.height = GLYPH_HEIGHT + (GLYPH_HEIGHT * element_count) + (element_count * 2) + 7;

    for(int i = 0; i < element_count; i++) {
        menu->node.width = MAX(menu->node.width, measure_str_width(elements[i].caption) + 4);
    }

    return menu;
}

void unload_ui_menu(ui_menu_t* menu) {
    free(menu->elements);
    free(menu);
}

void draw_ui_menu(ui_menu_t* menu) {
    int x = ((SCREEN_WIDTH - DOCK_WIDTH) >> 1) - (menu->node.width >> 1);
    int y = (SCREEN_HEIGHT >> 1) - (menu->node.height >> 1);

    draw_rectangle_filled(x, y, menu->node.width, menu->node.height, COLOR_WHITE);
    draw_rectangle(x, y, menu->node.width, menu->node.height, COLOR_BLACK);

    draw_text(menu->title, x + (menu->node.width >> 1) - (measure_str_width(menu->title) >> 1), y + 2, COLOR_BLACK);
    draw_line(x, y + GLYPH_HEIGHT + 3, x + menu->node.width - 1, y + GLYPH_HEIGHT + 3, COLOR_BLACK);

    int elements_y = y + GLYPH_HEIGHT + 6;

    for(int i = 0; i < menu->element_count; i++) {
        if(i == menu->selected_element) { 
            draw_rectangle_filled(x + 2, elements_y + (GLYPH_HEIGHT * i) + i * 2 - 1, menu->node.width - 4, GLYPH_HEIGHT + 2, COLOR_BLACK);
        }

        draw_text(menu->elements[i].caption, x + 3, elements_y + (GLYPH_HEIGHT * i) + i * 2, (i == menu->selected_element ? COLOR_WHITE : COLOR_BLACK));
    }
}

void update_ui_menu(ui_menu_t* menu) {
    if(is_key_pressed(BUTTON_MULTIPLY)) {
        delete_node_from_screen_ptr((ui_node_t*)menu, true);
        return;
    }

    if(is_key_pressed(BUTTON_EIGHT) && menu->element_count > 1) {
        menu->selected_element = (menu->selected_element <= 0 ? menu->element_count - 1 : menu->selected_element - 1);
        menu->node.should_redraw = true;
    }

    if(is_key_pressed(BUTTON_TWO) && menu->element_count > 1) {
        menu->selected_element = (menu->selected_element >= menu->element_count - 1 ? 0 : menu->selected_element + 1);
        menu->node.should_redraw = true;
    }

    if((is_key_pressed(BUTTON_FIVE) || is_key_pressed(BUTTON_PLUS)) && menu->element_count > 1 && menu->elements[menu->selected_element].callback != NULL) {
        menu->elements[menu->selected_element].callback(menu);
    }
}