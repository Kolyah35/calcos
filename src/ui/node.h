#pragma once
#include <stdint.h>

typedef struct ui_node_t {
    int x;
    int y;
    int width;
    int height;

    void(*update)(struct ui_node_t* self);
    void(*draw)(struct ui_node_t* self);
} ui_node_t;