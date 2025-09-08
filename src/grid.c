#include <stdlib.h>
#include <stdbool.h> 
#include <SDL3/SDL.h>
#include <headers/grid.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

void SetBackground(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);
}

cell* CreateGrid(SDL_Renderer *renderer, int window_width, int window_height) {
    const int cell_total_size = GRID_CELL_SIZE + (GRID_CELL_BORDER_SIZE * 2);

    cell* cells = malloc((GRID_ROWS * GRID_COLUMNS) * sizeof(cell));
    for (int x = 0; x < GRID_ROWS; x++) {
        for (int y = 0; y < GRID_COLUMNS; y++) {
            cells[(x * GRID_COLUMNS) + y].start_x = GRID_PADDING_SIZE + (x * cell_total_size);
            cells[(x * GRID_COLUMNS) + y].start_y = GRID_PADDING_SIZE + (y * cell_total_size);
            cells[(x * GRID_COLUMNS) + y].size = cell_total_size;
            cells[(x * GRID_COLUMNS) + y].visited = false;
        }
    }

    return cells;
}

void DrawGrid(SDL_Renderer *renderer, cell* cells) {
    for (int x = 0; x < GRID_ROWS; x++) {
        for (int y = 0; y < GRID_COLUMNS; y++) {
            // ADD BORDERS
            DrawBorder(renderer, cells, x, y, North);
            DrawBorder(renderer, cells, x, y, South);
            DrawBorder(renderer, cells, x, y, East);
            DrawBorder(renderer, cells, x, y, West);

            // ADD SQUARE
            DrawSquare(renderer, cells, x, y);
        }
    }

    return;
}

void DrawBorder(SDL_Renderer *renderer, cell* cells, int x, int y, direction dir) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
    const int cell_size = cells[(x * GRID_COLUMNS) + y].size;
    const int border_size = cell_size + GRID_CELL_BORDER_SIZE;
    switch (dir)
    {
        case North:
            const SDL_FRect north = { 
                .x = cells[(x * GRID_COLUMNS) + y].start_x, 
                .y = cells[(x * GRID_COLUMNS) + y].start_y, 
                .w = border_size, .h = GRID_CELL_BORDER_SIZE 
            };
            SDL_RenderFillRect(renderer, &north);
            break;
        case South:
            const SDL_FRect south = { 
                .x = cells[(x * GRID_COLUMNS) + y].start_x, 
                .y = cells[(x * GRID_COLUMNS) + y].start_y + cell_size, 
                .w = border_size, .h = GRID_CELL_BORDER_SIZE
            };
            SDL_RenderFillRect(renderer, &south);
            break;
        case East:
            const SDL_FRect east = { 
                .x = cells[(x * GRID_COLUMNS) + y].start_x, 
                .y = cells[(x * GRID_COLUMNS) + y].start_y, 
                .w = GRID_CELL_BORDER_SIZE, .h = border_size
            };
            SDL_RenderFillRect(renderer, &east);
            break;
        case West:
            const SDL_FRect west = { 
                .x = cells[(x * GRID_COLUMNS) + y].start_x + cell_size, 
                .y = cells[(x * GRID_COLUMNS) + y].start_y,
                .w = GRID_CELL_BORDER_SIZE, .h = border_size
            };
            SDL_RenderFillRect(renderer, &west);
            break;
    }
}

void DrawSquare(SDL_Renderer *renderer, cell* cells, int x, int y) {
    const int square_size = cells[(x * GRID_COLUMNS) + y].size - GRID_CELL_BORDER_SIZE;
    SDL_FRect inner_square = { 
        .x = cells[(x * GRID_COLUMNS) + y].start_x + GRID_CELL_BORDER_SIZE, 
        .y = cells[(x * GRID_COLUMNS) + y].start_y + GRID_CELL_BORDER_SIZE, 
        .w = square_size, .h = square_size
    };
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &inner_square);
}