#include <image.h>
#include <platform.h>
#include <utils.h>
#include <types.h>
#include <gfx.h>
#include <stdlib.h>
#include <stdio.h>

ui_image_t* load_ui_image(uint8_t format, uint8_t* data, uint16_t width, uint16_t height) {
    ui_image_t* ret = (ui_image_t*)malloc(sizeof(ui_image_t));
    ret->node.type = UI_IMAGE;
    ret->node.width = width;
    ret->node.height = height;
    ret->image.format = format;
    ret->image.data = data;
    ret->image.width = width;
    ret->image.height = height;

    return ret;
}

void draw_ui_image(ui_image_t* image) {
    float scale_x = (float)image->image.width / (float)image->node.width;
    float scale_y = (float)image->image.height / (float)image->node.height;

    if(image->image.format == IMAGE_FORMAT_1BIT) {
        for(int y = 0; y < image->node.height; y++) {
            for(int x = 0; x < image->node.width; x++) {
                bool pixel = BIT_READ(image->image.data[(int)(y * scale_y) * (int)(image->image.width >> 3) + ((int)(x * scale_x) >> 3)], x % 8);

                draw_pixel(x, y, (pixel ? COLOR_WHITE : COLOR_BLACK));
            }
        }
    }
}

void unload_ui_image(ui_image_t* image) {
    free(image->image.data);
    free(image);
}