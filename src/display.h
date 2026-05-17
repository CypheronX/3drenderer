#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "vector.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

typedef uint32_t color_t;

typedef enum {
    CULL_NONE,
    CULL_BACKFACE
} CullMode;

typedef enum {
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE
} RenderMode;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;

bool initialize_window(void);
void draw_grid(void);
void draw_pixel(int x, int y, color_t);
void draw_line(int x0, int y0, int x1, int y1, color_t);
void draw_rect(int x, int y, int width, int height, color_t);
void draw_rect_outline(int x, int y, int width, int height, color_t);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t);
void render_color_buffer(void);
void clear_color_buffer(color_t);
void destroy_window(void);
float dot_product(vec3_t* triangle_vertices, vec3_t camera_position);

#endif
