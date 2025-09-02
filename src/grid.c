#include <stdlib.h>
#include <SDL3/SDL.h>
#include <headers/grid.h>
#include <maze.c>

void DrawGrid(SDL_Renderer *renderer, float window_width, float window_height) {
    const int grid_size = window_width - GRID_PADDING_SIZE;
    const int square_size = floor(grid_size / GRID_SQUARE_SIZE);
    const int array_length = pow(square_size, 2);

    int* cells = GenerateMaze(array_length);

    // SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE_FLOAT);

    SDL_FRect square = { .x = 0, .y = 0, .w = GRID_SQUARE_SIZE, .h = GRID_SQUARE_SIZE };
    for(square.x = GRID_PADDING_SIZE; square.x <= (window_width - GRID_PADDING_SIZE); square.x += GRID_SQUARE_SIZE) {
        for(square.y = GRID_PADDING_SIZE; square.y <= (window_height - GRID_PADDING_SIZE); square.y += GRID_SQUARE_SIZE) {
            int value = rand();
            SDL_SetRenderDrawColor(renderer, value, value, value, SDL_ALPHA_OPAQUE_FLOAT);
            SDL_RenderFillRect(renderer, &square);
        }
    }

    free(cells);
    return;
}
