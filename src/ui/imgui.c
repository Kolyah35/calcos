#include <imgui.h>
#include <gfx.h>
#include <font.h>
#include <utils.h>
#include <stdio.h>
#include <keyboard.h>
#include <display.h>
#include <icons.h>

imgui_context_t ctx;

// all the colors are temp
#define SCREEN_PADDING 2
#define MAX_TEXT_BUF 32

void imgui_begin(const char* window_name) {
    ctx.last_x = SCREEN_PADDING;
    ctx.last_y = GLYPH_HEIGHT + 2;
    ctx.same_line = false;

    draw_text(window_name, 1, 1, COLOR_WHITE);
    draw_line(0, ctx.last_y, display_width, ctx.last_y, COLOR_WHITE);
    ctx.last_y += 2;
    ctx.last_id = 0;

    ctx.current_column = 0;
    ctx.current_row = 0;
}

void imgui_end() {
    if (ctx.selected_row > ctx.current_row) {
        ctx.selected_row = ctx.current_row;
    }
}

void imgui_same_line() {
    ctx.same_line = true;
    ctx.last_y -= ctx.last_h + SCREEN_PADDING;
    ctx.last_x += ctx.last_w + SCREEN_PADDING;
    ctx.current_column++;
}

int imgui_begin_element() {
    if (!ctx.same_line) {
        ctx.last_x = SCREEN_PADDING;

        if (ctx.selected_row == ctx.current_row && ctx.selected_column > ctx.current_column) {
            ctx.selected_column = ctx.current_column;
        }

        ctx.current_column = 0;
        ctx.current_row++;
        ctx.line_h = 0;
    }

    return ctx.last_id++;
}

void imgui_end_element() {
    if (ctx.current_column == ctx.selected_column && ctx.current_row == ctx.selected_row) {
        ctx.active_id = ctx.last_id - 1;
        draw_rect(ctx.last_x - 1, ctx.last_y - 1, ctx.last_w + 2, ctx.last_h + 2, COLOR_WHITE);
    }

    if (ctx.last_h > ctx.line_h) {
        ctx.line_h = ctx.last_h;
    }

    ctx.last_y += ctx.line_h + SCREEN_PADDING;
    ctx.same_line = false;
}

void imgui_update_ctx() {
    int prev_row = ctx.selected_row;
    int prev_col = ctx.selected_column;

    if (is_key_pressed(BUTTON_UP)) {
        ctx.selected_row--;
    }

    if (is_key_pressed(BUTTON_DOWN)) {
        ctx.selected_row++;
    }

    if (is_key_pressed(BUTTON_LEFT) && ctx.selected_column > 0) {
        ctx.selected_column--;
    }

    if (is_key_pressed(BUTTON_RIGHT)) {
        ctx.selected_column++;
    }

    if (prev_col != ctx.selected_column || prev_row != ctx.selected_row || is_key_pressed(BUTTON_OK)) {
        ctx.should_redraw = true;
    }
}

bool imgui_should_redraw() {
    return ctx.should_redraw;
}

void imgui_text(const char* fmt, ...) {
    imgui_begin_element();

    char buf[MAX_TEXT_BUF];
    va_list argptr;

    va_start(argptr, fmt);
    vsnprintf(buf, MAX_TEXT_BUF, fmt, argptr);
    va_end(argptr);
    
    draw_text(buf, ctx.last_x, ctx.last_y, COLOR_WHITE);

    ctx.last_w = measure_str_width(buf);
    ctx.last_h = GLYPH_HEIGHT;
    imgui_end_element();
}

void imgui_separator_text(const char* text) {
    ctx.same_line = false;
    
    imgui_begin_element();
    int y = ctx.last_y + (GLYPH_HEIGHT >> 1);
    int textw = measure_str_width(text);
    int line1_x = ctx.last_x;
    int line2_x = line1_x + textw + 5;
    int line1_w = 3;
    // temp
    int line2_w = display_width;

    draw_line(line1_x, y, line1_x + line1_w, y, COLOR_WHITE);
    draw_text(text, ctx.last_x + line1_w + 2, ctx.last_y, COLOR_WHITE);
    draw_line(line2_x, y, line2_x + line2_w, y, COLOR_WHITE);

    ctx.last_w = 0;
    ctx.last_h = GLYPH_HEIGHT;
    imgui_end_element();
}

bool imgui_button(const char* text) {
    int id = imgui_begin_element();

    int btn_w = measure_str_width(text) + 4;
    int btn_h = GLYPH_HEIGHT + 4;

    draw_rect(ctx.last_x, ctx.last_y, btn_w, btn_h, COLOR_WHITE);
    draw_text(text, ctx.last_x + 2, ctx.last_y + 2, COLOR_WHITE);

    ctx.last_w = btn_w;
    ctx.last_h = btn_h;

    imgui_end_element();
    
    return ctx.active_id == id && is_key_pressed(BUTTON_OK);
}

bool imgui_checkbox(const char* text, bool* value) {
    int id = imgui_begin_element();
    bool ret = false;

    const int check_w = 6;
    const int check_h = 6;

    void(*draw_check)(int, int, int, int, color_t) = (*value) ? fill_rect : draw_rect;
    
    draw_check(ctx.last_x, ctx.last_y, check_w, check_h, COLOR_WHITE);
    draw_text(text, ctx.last_x + check_w + 2, ctx.last_y, COLOR_WHITE);

    ctx.last_w = measure_str_width(text) + 4 + check_w;
    ctx.last_h = MAX(check_h, GLYPH_HEIGHT);

    if (ctx.active_id == id && is_key_pressed(BUTTON_OK)) {
        *value ^= 1;
        ret = true;
    }
    
    imgui_end_element();

    return ret;
}

bool imgui_radio_button(const char* text, int* value, int self_index) {
    int id = imgui_begin_element();
    bool ret = false;

    const int check_w = 6;
    const int check_h = 6;

    // void(*draw_check)(int, int, int, int, color_t) = (*value == self_index) ? fill_rect : draw_rect;
    
    draw_rect(ctx.last_x, ctx.last_y, check_w, check_h, COLOR_WHITE);
    draw_text(text, ctx.last_x + check_w + 2, ctx.last_y, COLOR_WHITE);

    if (*value == self_index) {
        fill_rect(ctx.last_x + 2, ctx.last_y + 2, check_w - 4, check_h - 4, COLOR_WHITE);
    }

    ctx.last_w = measure_str_width(text) + 4 + check_w;
    ctx.last_h = MAX(check_h, GLYPH_HEIGHT);
    
    if (ctx.active_id == id && is_key_pressed(BUTTON_OK)) {
        *value = self_index;
        ret = true;
    }

    imgui_end_element();

    return ret;
}

bool imgui_arrow_button(imgui_dir_t direction) {
    int id = imgui_begin_element();
	eicon_t icon_type = (direction == IMGUI_DIR_LEFT) ? ICON_ARROW_LEFT : ICON_ARROW_RIGHT;
	const icon_t* icon = get_icon(icon_type);

    // icon_t icon = {0, 2, 4}; 
    // icon.data = (direction == IMGUI_DIR_LEFT) ? 0b10111110 : 0b01111101;

    // draw_rect(ctx.last_x, ctx.last_y, 4, 6, COLOR_WHITE);
    draw_icon(icon, ctx.last_x + 1, ctx.last_y + 1, COLOR_WHITE);

    ctx.last_w = 4;
    ctx.last_h = 6;
    
    imgui_end_element();

    return ctx.active_id == id && is_key_pressed(BUTTON_FIVE);
}

void imgui_tooltip(const char* text) {
    int id = imgui_begin_element();
    int w = measure_str_width(text);
    int h = GLYPH_HEIGHT;
    
    if (ctx.active_id == id - 1) {
        fill_rect(ctx.last_x, ctx.last_y, w + 4, h + 4, COLOR_BLACK);
        draw_rect(ctx.last_x, ctx.last_y, w + 4, h + 4, COLOR_WHITE);
        draw_text(text, ctx.last_x + 2, ctx.last_y + 2, COLOR_WHITE);
    }

    ctx.last_w = 0;
    ctx.last_h = 0;

    imgui_end_element();
}
