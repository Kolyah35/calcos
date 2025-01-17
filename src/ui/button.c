#include <button.h>
#include <stdlib.h>
#include <string.h>
#include <font.h>
#include <gfx.h>

ui_button_t* load_ui_button(const char* text) {
    ui_button_t* ret = (ui_button_t*)malloc(sizeof(ui_button_t));
    load_node((ui_node_t*)ret, UI_BUTTON);
    ret->node.height = GLYPH_HEIGHT + 4;
    ret->text = text;

    return ret;
}

void draw_ui_button(ui_button_t* btn) {
    int strw = measure_str_width(btn->text);

    draw_rectangle(btn->node.x, btn->node.y, btn->node.width, btn->node.height, COLOR_BLACK);
    draw_text(btn->text, btn->node.x + (btn->node.width >> 1) - (strw >> 1), btn->node.y + (btn->node.height >> 1) - (GLYPH_HEIGHT >> 1), COLOR_BLACK);
}


void unload_ui_button(ui_button_t* btn) {
    free((void*)btn->text);
}