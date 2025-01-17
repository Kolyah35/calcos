#pragma once
#include <node.h>

typedef struct {
    ui_node_t node;
    const char* text;
} ui_button_t;

ui_button_t* load_ui_button(const char* text);
void draw_ui_button(ui_button_t* btn);
void unload_ui_button(ui_button_t* btn);