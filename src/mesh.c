#include "mesh.h"

face_t mesh_faces[N_MESH_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3 },
    // { .a = 1, .b = 3, .c = 4 },
    // right
    { .a = 3, .b = 1, .c = 5 },
    // { .a = 4, .b = 5, .c = 6 },
    // back
    { .a = 1, .b = 5, .c = 4 },
    // { .a = 6, .b = 7, .c = 8 },
    // left
    { .a = 4, .b = 1, .c = 2 },
    // { .a = 8, .b = 2, .c = 1 },
    // top
    // { .a = 2, .b = 7, .c = 5 },
    // { .a = 2, .b = 5, .c = 3 },
    // bottom
    { .a = 2, .b = 3, .c = 5 },
    { .a = 2, .b = 4, .c = 5 },
};

vec3_t mesh_vertices[N_MESH_VERTICES] = {
    // { .x = -1, .y = -1, .z = -1 },
    // { .x = -1, .y = 1, .z = -1 },
    // { .x = 1, .y = 1, .z = -1 },
    { .x = 0, .y = 1, .z = 0 },
    { .x = -1, .y = -1, .z = 1 },
    { .x = 1, .y = -1, .z = 1 },
    { .x = -1, .y = -1, .z = -1 },
    { .x = 1, .y = -1, .z = -1 },
};
