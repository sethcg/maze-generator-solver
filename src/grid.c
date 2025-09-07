#include <stdlib.h>
#include <SDL3/SDL.h>
#include <headers/grid.h>
#include <maze.c>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

void DrawGrid(SDL_Renderer *renderer, int window_width, int window_height) {
    // SET BACKGROUND COLOR (DARK GRAY)
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    const int window_size = min(window_width, window_height);
    const int grid_max_size = (window_size - GRID_PADDING_MINIMUM_SIZE);
    
    const int grid_square_max_size = (grid_max_size - (GRID_SQUARE_NUMBER * (GRID_BORDER_SIZE * 2))) / GRID_SQUARE_NUMBER;
    const int grid_square_size = max(GRID_SQUARE_MINIMUM_SIZE, grid_square_max_size);
    const int grid_square_total_size = grid_square_size + (GRID_BORDER_SIZE * 2);
    const int grid_size = (grid_square_total_size * GRID_SQUARE_NUMBER);

    const int array_length = pow(grid_size, 2);
    int* cells = GenerateMaze(array_length);

    const int grid_padding_x = max(GRID_PADDING_MINIMUM_SIZE, (window_width - grid_size) / 2);
    const int grid_padding_y = max(GRID_PADDING_MINIMUM_SIZE, (window_height - grid_size) / 2);
    const int grid_start_x = grid_padding_x + GRID_BORDER_SIZE;
    const int grid_start_y = grid_padding_y + GRID_BORDER_SIZE;

    SDL_FRect square = { .x = 0, .y = 0, .w = grid_square_size, .h = grid_square_size };
    for(square.x = grid_start_x; square.x <= ((grid_start_x + grid_size) - grid_square_total_size); square.x += grid_square_size + GRID_BORDER_SIZE) {
        for(square.y = grid_start_y; square.y <= ((grid_start_y + grid_size) - grid_square_total_size); square.y += grid_square_size + GRID_BORDER_SIZE) {
            int tempX = square.x - GRID_BORDER_SIZE;
            int tempY = square.y - GRID_BORDER_SIZE;
            
            // TOP BORDER, ONLY ON THE FIRST ROW
            if(square.y == grid_start_y) {
                SDL_FRect top_border = { .x = tempX, .y = tempY, .w = grid_square_total_size, .h = GRID_BORDER_SIZE };
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
                SDL_RenderFillRect(renderer, &top_border);
            }

            // BOTTOM BORDER
            SDL_FRect bottom_border = { .x = tempX, .y = tempY + grid_square_size + GRID_BORDER_SIZE, .w = grid_square_total_size, .h = GRID_BORDER_SIZE };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
            SDL_RenderFillRect(renderer, &bottom_border);

            // LEFT BORDER, ONLY ON THE FIRST COLUMN
            if(square.x == grid_start_x) {
                SDL_FRect left_border = { .x = tempX, .y = tempY, .w = GRID_BORDER_SIZE, .h = grid_square_total_size };
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
                SDL_RenderFillRect(renderer, &left_border);
            }

            // RIGHT BORDER
            SDL_FRect right_border = { .x = tempX + grid_square_size + GRID_BORDER_SIZE, .y = tempY, .w = GRID_BORDER_SIZE, .h = grid_square_total_size };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
            SDL_RenderFillRect(renderer, &right_border);

            // ADD SQUARE
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE_FLOAT);
            SDL_RenderFillRect(renderer, &square);
        }
    }

    free(cells);
    return;
}
