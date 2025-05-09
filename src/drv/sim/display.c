#include <config.h>

#ifdef PLATFORM_SIM

#include <raylib.h>
#include <raymath.h>
#include <display.h>

#ifndef SIM_DISPLAY_WIDTH
    #define SIM_DISPLAY_WIDTH DISPLAY_WIDTH
#endif

#ifndef SIM_DISPLAY_HEIGHT
    #define SIM_DISPLAY_HEIGHT DISPLAY_HEIGHT
#endif

int display_width = SIM_DISPLAY_WIDTH;
int display_height = SIM_DISPLAY_HEIGHT;

static inline Color convert_to_raylib_color(pixel_color_t color) {
	Color rcolor = {0, 0, 0, 255};
	
#if DISPLAY_COLOR_DEPTH == 1
	rcolor = (color) ? WHITE : BLACK;
#elif DISPLAY_COLOR_DEPTH == 2
	static Color color_array[] = {
		BLACK, SKYBLUE, MAGENTA, GRAY	
	};

	if (color > 3) color = 0;

	rcolor = color_array[color];
#elif DISPLAY_COLOR_DEPTH == 15
	rcolor.r = (color & 0b0111110000000000) >> 8;
	rcolor.g = (color & 0b0000001111100000) >> 3;
	rcolor.b = (color & 0b0000000000011111) << 3
#elif DISPLAY_COLOR_DEPTH == 16
	rcolor.r = (color & 0b1111100000000000) >> 8;
	rcolor.g = (color & 0b0000011111100000) >> 3;
	rcolor.b = (color & 0b0000000000011111) << 3;
#elif DISPLAY_COLOR_DEPTH == 24
	rcolor.r = ((uint8_t*)&color)[1];
	rcolor.g = ((uint8_t*)&color)[2];
	rcolor.b = ((uint8_t*)&color)[3];
#endif

	return rcolor;
}

void display_init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(display_width * 2, display_height * 2, "CalcOS Simulator");
    SetWindowMinSize(display_width, display_height);
    SetTargetFPS(60);

    framebuffer = LoadRenderTexture(display_width, display_height);
}

void display_set_contrast(uint8_t val) {
    return; // not supported yet
}

void display_clear() {
	BeginTextureMode(framebuffer);
		ClearBackground(BLACK);
	EndTextureMode();
}

void display_set_pixel(int x, int y, pixel_color_t color) {
    DrawPixel(x, y, convert_to_raylib_color(color));
}

void display_update(void) {
    float scale = MIN((float)GetScreenWidth() / display_width, (float)GetScreenHeight() / display_height);

    BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(
            framebuffer.texture, 
            (Rectangle){0.0f, 0.0f, (float)framebuffer.texture.width, (float)-framebuffer.texture.height},
            (Rectangle){(GetScreenWidth() - ((float)display_width * scale)) * 0.5f, (GetScreenHeight() - ((float)display_height * scale)) * 0.5f,
                        (float)display_width * scale, (float)display_height * scale},
            Vector2Zero(), 0.0f, WHITE
        );
    EndDrawing();
}

void display_draw_rect_filled(int x, int y, int w, int h, pixel_color_t color) {
    DrawRectangle(x, y, w, h, convert_to_raylib_color(color));
}

void display_draw_line_vertical(int x, int y, int h, pixel_color_t color) {
    DrawLine(x, y, x, y + h, convert_to_raylib_color(color));
}

void display_draw_line_horizontal(int x, int y, int w, pixel_color_t color) {
    DrawLine(x, y, x + w, y, convert_to_raylib_color(color));
}

#endif // PLATFORM_SIM
