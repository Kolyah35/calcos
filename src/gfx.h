#pragma once
#include <types.h>
#include <icons.h>

void set_clip(int x, int y, int width, int height);
void translate(int x, int y);

void draw_pixel(int x, int y, color_t color);

void draw_line(int x1, int y1, int x2, int y2, color_t color);
void draw_rect(int x, int y, int width, int height, color_t color);
void fill_rect(int x, int y, int width, int height, color_t color);
void draw_text(const char* str, int x, int y, color_t color);
void draw_icon(const icon_t* icon, int x, int y, color_t color);
// void draw_image(image_t image, int x, int y);
