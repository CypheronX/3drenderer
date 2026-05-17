#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "display.h"
#include "vector.h"
#include <stdint.h>

typedef struct {
    int a;
    int b;
    int c;
    color_t color;
} face_t;

typedef struct {
    vec2_t points[3];
    color_t color;
} triangle_t;

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t);
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t);
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t);

#endif
