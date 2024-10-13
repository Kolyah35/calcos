#pragma once
#include <node.h>
#include <types.h>

typedef struct ui_text_t {
    ui_node_t node;

    const char* text;
    color_t color;
    int scroll_position;
    float scroll_time;
} ui_text_t;

typedef struct ui_text_min_t {
    node_type_t type;

    int x;
    int y;

    char* text;
    color_t color;
} ui_text_min_t;

ui_text_t* load_ui_text(color_t color, const char* fmt, ...);
ui_text_min_t* load_ui_text_min(color_t color, const char* fmt, ...);

void update_ui_text(ui_node_t* self);

void draw_ui_text(ui_node_t* self);
void draw_ui_text_min(ui_text_min_t* self);