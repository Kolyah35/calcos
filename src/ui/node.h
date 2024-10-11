#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct screen_t screen_t;
typedef struct ui_node_t ui_node_t;

typedef void(*node_parent_callback_t)(ui_node_t* self, screen_t* parent);
typedef void(*node_callback_t)(ui_node_t* self);

typedef struct ui_node_t {
    node_callback_t unload;
    node_callback_t draw;
    node_parent_callback_t update;

    bool should_redraw;

    int x;
    int y;
    int width;
    int height;
} ui_node_t;