#pragma once
#include <stdbool.h>

typedef struct {
    int last_x;
    int last_y;
    int last_w;
    int last_h;

    int last_id;
    int active_id;

    int current_row;
    int current_column;

    int selected_row;
    int selected_column;

    int line_h;
    
    bool same_line;
    bool should_redraw;
} imgui_context_t;

typedef enum {IMGUI_DIR_LEFT, IMGUI_DIR_RIGHT} imgui_dir_t;

void imgui_begin(const char* window_name);
void imgui_end();

void imgui_same_line();
int imgui_begin_element();
void imgui_end_element();
void imgui_update_ctx();
bool imgui_should_redraw();

void imgui_text(const char* fmt, ...);
void imgui_separator_text(const char* text);
bool imgui_button(const char* text);
bool imgui_arrow_button(imgui_dir_t direction);
bool imgui_checkbox(const char* text, bool* value);
bool imgui_radio_button(const char* text, int* value, int self_index);
void imgui_tooltip(const char* text);