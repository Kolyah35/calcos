#include <text.h>
#include <stdlib.h>
#include <gfx.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_LENGTH 256

ui_text_min_t* load_ui_text_min(color_t color, const char* fmt, ...) {
    ui_text_min_t* ret = malloc(sizeof(ui_text_min_t));
    ret->type = UI_TEXT_MIN;
    ret->color = color;

    if(fmt != NULL) {
        char* buf = malloc(MAX_BUFFER_LENGTH);
        memset(buf, 0, MAX_BUFFER_LENGTH);

        va_list argptr;
        va_start(argptr, fmt);
        snprintf(buf, MAX_BUFFER_LENGTH, fmt, argptr);
        va_end(argptr);

        buf = realloc(buf, strlen(buf));

        ret->text = buf;
    }

    return ret;
}

void draw_ui_text_min(ui_text_min_t* self) {
    if(self->text != NULL) {
        draw_text(self->text, self->x, self->y, self->color);
    }
}