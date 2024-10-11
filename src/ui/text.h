#pragma once
#include <node.h>
#include <types.h>

typedef struct ui_text_t {
    ui_node_t node;

    const char* text;
    display_color_t color;
    int scroll_position;
    float scroll_time;
} ui_text_t; // 24 байт

typedef struct ui_text_min_t {
    node_callback_t unload;
    node_callback_t draw;

    int x;
    int y;

    char* text;
    display_color_t color;
} ui_text_min_t; // 11 байт

ui_text_t* load_ui_text(display_color_t color, const char* fmt, ...);
ui_text_min_t* load_ui_text_min(display_color_t color, const char* fmt, ...);

void update_ui_text(ui_node_t* self);

void draw_ui_text(ui_node_t* self);
void draw_ui_text_min(ui_text_min_t* self);