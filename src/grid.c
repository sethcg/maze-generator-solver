#include <stdlib.h>
#include <SDL3/SDL.h>
#include <headers/grid.h>
#include <maze.c>

#define min(a,b) (((a) < (b)) ? (a) : (b))

void DrawGrid(SDL_Renderer *renderer, int window_width, int window_height) {
    // SET BACKGROUND COLOR (DARK GRAY)
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    const int grid_width = (window_width - GRID_PADDING_SIZE);
    const int grid_height = (window_height - GRID_PADDING_SIZE);
    const int grid_square_size = min(grid_width, grid_height) / GRID_SQUARE_NUMBER;
    const int array_length = grid_width * grid_height;

    int* cells = GenerateMaze(array_length);

    // SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_FRect square = { .x = 0, .y = 0, .w = grid_square_size, .h = grid_square_size };
    for(square.x = GRID_PADDING_SIZE; square.x <= (grid_width - grid_square_size); square.x += grid_square_size) {
        for(square.y = GRID_PADDING_SIZE; square.y <= (grid_height - grid_square_size); square.y += grid_square_size) {
            int value = rand();
            SDL_SetRenderDrawColor(renderer, value, value, value, SDL_ALPHA_OPAQUE_FLOAT);
            SDL_RenderFillRect(renderer, &square);
        }
    }

    free(cells);
    return;
}
