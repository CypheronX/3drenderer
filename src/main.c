#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"

triangle_t* triangles_to_render = NULL;

vec3_t camera_position = { 0, 0, 0 };
float fov_factor = 640;

bool is_running = false;
int previous_frame_time = 0;

RenderMode render_method;
CullMode cull_method;

void setup(void) {
    // Initialize render mode and triangle culling method
    render_method = RENDER_WIRE;
    cull_method = CULL_BACKFACE;

    // Allocate the required memory in bytes to hold the color buffer 
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    if (color_buffer == NULL) {
        fprintf(stderr, "Failed to allocate color_buffer\n");
        exit(EXIT_FAILURE);
    }

    // Creating a SDL texture that is used to display color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );

    // Loads the cube values in the mesh data structure
    load_cube_mesh_data();
    // load_obj_file_data("/home/alex/code/3drenderer/assets/cube.obj");
}

void process_input(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        is_running = false;
                        break;

                    case SDLK_1:
                        render_method = RENDER_WIRE_VERTEX;
                        break;
                        
                    case SDLK_2:
                        render_method = RENDER_WIRE;
                        break;

                    case SDLK_3:
                        render_method = RENDER_FILL_TRIANGLE;
                        break;

                    case SDLK_4:
                        render_method = RENDER_FILL_TRIANGLE_WIRE;
                        break;

                    case SDLK_c:
                        cull_method = CULL_BACKFACE;
                        break;

                    case SDLK_d:
                        cull_method = CULL_NONE;
                        break;
                }
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////////
// Function that receives a 3D vector and returns a projected 2D point
//////////////////////////////////////////////////////////////////////////////////////
vec2_t project(vec3_t point) {
   vec2_t projected_point = {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z
   }; 

   return projected_point;
}

void update(void) {

    int time_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    if (time_wait > 0 && time_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_wait);
    }

    previous_frame_time = SDL_GetTicks();

    // Initialize the array of triangles to renderer
    triangles_to_render = NULL;

    mesh.rotation.x += 0.01;
    mesh.rotation.y += 0.02;
    mesh.rotation.z += 0.01;

    // Loop all triangle faces
    for (int i = 0; i < array_length(mesh.faces); i++) {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[N_FACE_VERTICES];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec3_t transformed_vertices[N_FACE_VERTICES];

        // Loop all three vertices of this current face and apply transformations
        for (int y = 0; y < N_FACE_VERTICES; y++) {
            vec3_t transformed_vertex = face_vertices[y];

            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // Translate the vertex away from the camera in z
            transformed_vertex.z += 5;

            // Save transformed vertex in the array of transformed vertices
            transformed_vertices[y] = transformed_vertex;
        }

        // Bypass the triangles that are looking away from the camera
        if (cull_method == CULL_BACKFACE && dot_product(transformed_vertices, camera_position) < 0) {
            continue;
        } 

        vec2_t projected_points[N_FACE_VERTICES];

        // Loop all three vertices to perform the projection
        for (int y = 0; y < N_FACE_VERTICES; y++) {
            // Project the current vertex
            projected_points[y] = project(transformed_vertices[y]);

            // Scale and translate the projected points to the middle of the screen
            projected_points[y].x += (window_width / 2);
            projected_points[y].y += (window_height / 2);
        }

        triangle_t projected_triangle = {
            .points = {
                { projected_points[0].x, projected_points[0].y },
                { projected_points[1].x, projected_points[1].y },
                { projected_points[2].x, projected_points[2].y },
            },
            .color = mesh_face.color
        };

        // Save the projected triangle in the array of triangles to render
        array_push(triangles_to_render, projected_triangle);
    }
}

void render(void) {
    draw_grid();

    // Loop all projected triangles and render them
    int num_trinagles = array_length(triangles_to_render);
    for (int i = 0; i < num_trinagles; i++) {
        triangle_t triangle = triangles_to_render[i];

        if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
            draw_filled_triangle(
                triangle.points[0].x, 
                triangle.points[0].y, 
                triangle.points[1].x, 
                triangle.points[1].y, 
                triangle.points[2].x, 
                triangle.points[2].y, 
                triangle.color
            );
        }

        if (render_method == RENDER_WIRE || render_method == RENDER_FILL_TRIANGLE_WIRE || render_method == RENDER_WIRE_VERTEX) {
            draw_triangle(
                triangle.points[0].x, 
                triangle.points[0].y, 
                triangle.points[1].x, 
                triangle.points[1].y, 
                triangle.points[2].x, 
                triangle.points[2].y, 
                0x00E5FF
            );
        }

        if (render_method == RENDER_WIRE_VERTEX) {
            draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFFFF);
            draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFFFF);
            draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFFFF);
        }
    }

    // Clear the array of triangles to render every frame loop
    array_free(triangles_to_render);

    render_color_buffer();

    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

// Free memory that was dynamically allocated by the program
void free_resources(void) {
   free(color_buffer);
   array_free(mesh.faces); 
   array_free(mesh.vertices); 
}

int main(void) {

    is_running = initialize_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();
    free_resources();

	return 0;
}
