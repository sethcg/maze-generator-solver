#include <SDL3/SDL.h>
#include <headers/grid.h>

void DrawGrid(SDL_Renderer *renderer, float window_width, float window_height) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE_FLOAT);

    // ADD ROWS
    SDL_FRect row_line = { .x = GRID_PADDING_SIZE, .y = 0, .w = (window_width - (GRID_PADDING_SIZE * 2)), .h = 0 };
    for(row_line.y = GRID_PADDING_SIZE; row_line.y <= (window_height - GRID_PADDING_SIZE); row_line.y += GRID_SQUARE_SIZE) {
        SDL_RenderRect(renderer, &row_line);
    }

    // ADD COLUMNS
    SDL_FRect col_line = { .x = 0, .y = GRID_PADDING_SIZE, .w = 0, .h = (window_height - (GRID_PADDING_SIZE * 2)) };
    for(col_line.x = GRID_PADDING_SIZE; col_line.x <= (window_width - GRID_PADDING_SIZE); col_line.x += GRID_SQUARE_SIZE) {
        SDL_RenderRect(renderer, &col_line);
    }
}
