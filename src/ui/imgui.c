#include <imgui.h>
#include <gfx.h>
#include <font.h>
#include <utils.h>
#include <stdio.h>
#include <keyboard.h>

imgui_context_t ctx;

// all the colors are temp
#define SCREEN_PADDING 1
#define MAX_TEXT_BUF 32

void imgui_begin(const char* window_name) {
    ctx.last_x = SCREEN_PADDING;
    ctx.last_y = GLYPH_HEIGHT + 2;
    ctx.same_line = false;

    draw_text(window_name, 1, 1, COLOR_BLACK);
    draw_line(0, ctx.last_y, DISPLAY_WIDTH, ctx.last_y, COLOR_BLACK);
    ctx.last_y += 2;
    ctx.last_id = 0;

    if (ctx.active_id < 0) {
        ctx.active_id = 0;
    }
}

void imgui_end() {
    if (ctx.active_id > ctx.last_id) {
        ctx.active_id = ctx.last_id;
    }
}

void imgui_same_line() {
    ctx.same_line = true;
    ctx.last_y -= ctx.last_h + SCREEN_PADDING;
    ctx.last_x += ctx.last_w + SCREEN_PADDING;
}


int imgui_begin_ctx() {
    if (!ctx.same_line) {
        ctx.last_x = SCREEN_PADDING;
    }

    return ctx.last_id++;
}

void imgui_end_ctx() {
    if (ctx.active_id == ctx.last_id - 1) {
        draw_rectangle(ctx.last_x - 1, ctx.last_y - 1, ctx.last_w + 2, ctx.last_h + 2, COLOR_BLACK);
    }

    ctx.last_y += ctx.last_h + SCREEN_PADDING;
    ctx.same_line = false;
}

void imgui_update_ctx() {
    int prev_id = ctx.active_id;

    if (is_key_pressed(BUTTON_UP)) {
        ctx.active_id--;
    }

    if (is_key_pressed(BUTTON_DOWN)) {
        ctx.active_id++;
    }

    if (prev_id != ctx.active_id || is_key_pressed(BUTTON_OK)) {
        ctx.should_redraw = true;
    }
}

bool imgui_should_redraw() {
    return ctx.should_redraw;
}

void imgui_text(const char* fmt, ...) {
    imgui_begin_ctx();

    char buf[MAX_TEXT_BUF];
    va_list argptr;

    va_start(argptr, fmt);
    vsnprintf(buf, MAX_TEXT_BUF, fmt, argptr);
    va_end(argptr);
    
    draw_text(buf, ctx.last_x, ctx.last_y, COLOR_BLACK);

    ctx.last_w = measure_str_width(buf);
    ctx.last_h = GLYPH_HEIGHT;
    imgui_end_ctx();
}

void imgui_separator_text(const char* text) {
    imgui_begin_ctx();
    int y = ctx.last_y + (GLYPH_HEIGHT >> 1);
    int textw = measure_str_width(text);
    int line1_x = ctx.last_x;
    int line2_x = line1_x + textw + 5;
    int line1_w = 3;
    // temp
    int line2_w = DISPLAY_WIDTH;

    draw_line(line1_x, y, line1_x + line1_w, y, COLOR_BLACK);
    draw_text(text, ctx.last_x + line1_w + 2, ctx.last_y, COLOR_BLACK);
    draw_line(line2_x, y, line2_x + line2_w, y, COLOR_BLACK);

    ctx.last_w = 0;
    ctx.last_h = GLYPH_HEIGHT;
    imgui_end_ctx();
}

bool imgui_button(const char* text) {
    int id = imgui_begin_ctx();

    int btn_w = measure_str_width(text) + 4;
    int btn_h = GLYPH_HEIGHT + 4;

    draw_rectangle(ctx.last_x, ctx.last_y, btn_w, btn_h, COLOR_BLACK);
    draw_text(text, ctx.last_x + 2, ctx.last_y + 2, COLOR_BLACK);

    ctx.last_w = btn_w;
    ctx.last_h = btn_h;

    imgui_end_ctx();
    
    return ctx.active_id == id && is_key_pressed(BUTTON_OK);
}

bool imgui_checkbox(const char* text, bool* value) {
    int id = imgui_begin_ctx();
    bool ret = false;

    int check_w = 6;
    int check_h = 6;
    icon_t checked_icon = {0b001001011000, 4, 3};
    
    draw_rectangle(ctx.last_x, ctx.last_y, check_w, check_h, COLOR_BLACK);
    draw_text(text, ctx.last_x + check_w + 2, ctx.last_y, COLOR_BLACK);

    if (*value) {
        draw_icon(checked_icon, ctx.last_x + 1, ctx.last_y + 2, COLOR_BLACK);
    }

    ctx.last_w = measure_str_width(text) + 4 + check_w;
    ctx.last_h = MAX(check_h, GLYPH_HEIGHT);

    if (ctx.active_id == id && is_key_pressed(BUTTON_OK)) {
        *value ^= 1;
        ret = true;
    }
    
    imgui_end_ctx();

    return ret;
}

bool imgui_radio_button(const char* text, int* value, int self_index) {
    int id = imgui_begin_ctx();
    bool ret = false;

    int check_w = 6;
    int check_h = 6;
    icon_t checked_icon = {0b0000011001100000, 4, 4};
    
    draw_rectangle(ctx.last_x, ctx.last_y, check_w, check_h, COLOR_BLACK);
    draw_text(text, ctx.last_x + check_w + 2, ctx.last_y, COLOR_BLACK);

    if (*value == self_index) {
        draw_icon(checked_icon, ctx.last_x + 1, ctx.last_y + 1, COLOR_BLACK);
    }

    ctx.last_w = measure_str_width(text) + 4 + check_w;
    ctx.last_h = MAX(check_h, GLYPH_HEIGHT);
    
    if (ctx.active_id == id && is_key_pressed(BUTTON_OK)) {
        *value = self_index;
        ret = true;
    }

    imgui_end_ctx();

    return ret;
}

bool imgui_arrow_button(imgui_dir_t direction) {
    int id = imgui_begin_ctx();

    icon_t icon = {0, 2, 4}; 
    icon.data = (direction == IMGUI_DIR_LEFT) ? 0b10111110 : 0b01111101;

    draw_rectangle_filled(ctx.last_x, ctx.last_y, 4, 6, COLOR_BLACK);
    draw_icon(icon, ctx.last_x + 1, ctx.last_y + 1, COLOR_WHITE);

    ctx.last_w = 4;
    ctx.last_h = 6;
    
    imgui_end_ctx();

    return ctx.active_id == id && is_key_pressed(BUTTON_FIVE);
}

void imgui_tooltip(const char* text) {
    int id = imgui_begin_ctx();
    int w = measure_str_width(text);
    int h = GLYPH_HEIGHT;
    
    if (ctx.active_id == id - 1) {
        draw_rectangle_filled(ctx.last_x, ctx.last_y, w + 4, h + 4, COLOR_BLACK);
        draw_rectangle(ctx.last_x, ctx.last_y, w + 4, h + 4, COLOR_BLACK);
        draw_text(text, ctx.last_x + 2, ctx.last_y + 2, COLOR_BLACK);
    }

    ctx.last_w = 0;
    ctx.last_h = 0;

    imgui_end_ctx();
}