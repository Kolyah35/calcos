#pragma once
#include <types.h>

void set_draw_area(int x, int y, int width, int height);

inline vec2_t get_draw_area_size();
void draw_pixel(uint16_t x, uint16_t y, color_t color);

void draw_line(int x1, int y1, int x2, int y2, color_t color);
void draw_rectangle(int x, int y, int width, int height, color_t color);
void draw_rectangle_filled(int x, int y, int width, int height, color_t color);
void draw_text(const char* str, int x, int y, color_t color);