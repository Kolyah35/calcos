#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef enum node_type_t {
    UI_TEXT = 0,
    UI_TEXT_MIN,
    UI_MENU,
    UI_IMAGE,

    // clickable nodes
    UI_POPUP_MENU,
    UI_BUTTON
} node_type_t;

typedef struct ui_node_t {
    uint8_t type; // should be a first byte!

    int x;
    int y;
    int width;
    int height;

    bool should_redraw;
} ui_node_t;

void load_node(ui_node_t* node, node_type_t type);
void unload_node(ui_node_t* node);
bool is_node_selectable(node_type_t type);
bool is_node_auto_placeable(node_type_t type);