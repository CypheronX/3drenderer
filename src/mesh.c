#include <stdio.h>
#include <string.h>
#include "mesh.h"
#include "array.h"
#include "triangle.h"
#include "vector.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = { 0, 0, 0 }
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3, .color = 0xFFFF0000 },
    { .a = 1, .b = 3, .c = 4, .color = 0xFFFF0000 },
    // right
    { .a = 4, .b = 3, .c = 5, .color = 0xFF00FF00 },
    { .a = 4, .b = 5, .c = 6, .color = 0xFF00FF00 },
    // back
    { .a = 6, .b = 5, .c = 7, .color = 0xFF0000FF },
    { .a = 6, .b = 7, .c = 8, .color = 0xFF0000FF },
    // left
    { .a = 8, .b = 7, .c = 2, .color = 0xFFFFFF00 },
    { .a = 8, .b = 2, .c = 1, .color = 0xFFFFFF00 },
    // top
    { .a = 2, .b = 7, .c = 5, .color = 0xFFFF00FF },
    { .a = 2, .b = 5, .c = 3, .color = 0xFFFF00FF },
    // bottom
    { .a = 6, .b = 8, .c = 1, .color = 0xFF00FFFF },
    { .a = 6, .b = 1, .c = 4, .color = 0xFF00FFFF },
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    { .x = -1, .y = -1, .z = -1 },
    { .x = -1, .y = 1, .z = -1 },
    { .x = 1, .y = 1, .z = -1 },
    { .x = 1, .y = -1, .z = -1 },
    { .x = 1, .y = 1, .z = 1 },
    { .x = 1, .y = -1, .z = 1 },
    { .x = -1, .y = 1, .z = 1 },
    { .x = -1, .y = -1, .z = 1 }
};

void load_cube_mesh_data(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        array_push(mesh.vertices, cube_vertices[i]);
    }

    for (int i = 0; i < N_CUBE_FACES; i++) {
        array_push(mesh.faces, cube_faces[i]);
    }
}

void load_obj_file_data(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    char line[1024];

    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            size_t len = strlen(line);
            if (len > 0 && line[len-1] == '\n') {
                line[len-1] = '\0';
            }

            if (strncmp(line, "v ", 2) == 0) {
                vec3_t vertex;

                if (sscanf(line + 2, "%f %f %f", &vertex.x, &vertex.y, &vertex.z) == 3) {
                    array_push(mesh.vertices, vertex);
                }
            }

            if (strncmp(line, "f ", 2) == 0) {
                face_t face;

                if (sscanf(line + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &face.a, &face.b, &face.c) == 3) {
                    array_push(mesh.faces, face);
                }
            }
        }

        fclose(file);
    }
}
