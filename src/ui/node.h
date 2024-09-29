#pragma once
#include <stdint.h>

struct ui_node_t {
    int x;
    int y;
    int width;
    int height;

    void(*update)(ui_node_t* self);
    void(*draw)(ui_node_t* self);
};