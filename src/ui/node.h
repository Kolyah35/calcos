#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef enum node_type_t {
    UI_TEXT = 0,
    UI_TEXT_MIN,
    UI_MENU
} node_type_t;

typedef struct ui_node_t {
    node_type_t type; // should be a first byte!
    bool should_redraw;

    int x;
    int y;
    int width;
    int height;
} ui_node_t;

void unload_node(ui_node_t* node);