#pragma once
#include <stdint.h>

#define IMAGE_FORMAT_1BIT 0

typedef struct {
    uint8_t format;
    void* data;
    uint16_t width;
    uint16_t height;
} image_t;