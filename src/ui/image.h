#pragma once
#include <node.h>
#include <image.h>

#define IMAGE_FORMAT_1BIT 0

typedef struct {
    uint8_t format;
    uint8_t* data;
    uint16_t width;
    uint16_t height;
} image_t;

typedef struct {
    ui_node_t node;
    image_t image;
} ui_image_t;

ui_image_t* load_ui_image(uint8_t format, uint8_t* data, uint16_t width, uint16_t height);
void draw_ui_image(ui_image_t* image);
void unload_ui_image(ui_image_t* image);
