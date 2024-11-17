#include <popup_menu.h>
#include <font.h>
#include <gfx.h>
#include <stdlib.h>

ui_popup_menu_t* load_ui_popup_menu(const char** elements, int element_count) {
    ui_popup_menu_t* menu = (ui_popup_menu_t*)malloc(sizeof(ui_popup_menu_t));
    menu->elements = elements;
    menu->element_count = element_count;
    menu->selected_element = 0;
    menu->node.type = UI_MENU;
    menu->node.width = measure_str_width(elements[0]) + 10;
    menu->node.height = GLYPH_HEIGHT + 4;

    return menu;
}

void draw_ui_popup_menu(ui_popup_menu_t* popup_menu) {
    int width = measure_str_width(popup_menu->elements[popup_menu->selected_element]) + 11;
    int height = GLYPH_HEIGHT + 4;

    draw_rectangle(popup_menu->node.x, popup_menu->node.y, width, height, COLOR_BLACK);

    int line_x = popup_menu->node.x + width - 9;
    draw_line(line_x, popup_menu->node.y, line_x, popup_menu->node.y + height, COLOR_BLACK);
    draw_text(popup_menu->elements[popup_menu->selected_element], popup_menu->node.x + 2, popup_menu->node.y + 2, COLOR_BLACK);
    draw_icon(get_icon(ICON_MENU), line_x + 2, popup_menu->node.y + 2, COLOR_BLACK);
}

void unload_ui_popup_menu(ui_popup_menu_t* menu) {
    free(menu->elements);
    free(menu);
    menu = NULL;
}