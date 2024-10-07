#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef void(*node_callback_t)(struct ui_node_t* self); 

typedef struct ui_node_t {
    int x;
    int y;
    int width;
    int height;

    bool should_redraw;

    node_callback_t update;
    node_callback_t draw;
} ui_node_t;