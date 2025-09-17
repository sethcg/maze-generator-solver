#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <Grid.h>
#include <Cell.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

void CreateGrid(SDL_Renderer *renderer, cell* cells) {
    const int cell_size = GRID_CELL_SIZE + (GRID_CELL_BORDER_SIZE * 2);

    for (int x = 0; x < GRID_ROWS; x++) {
        for (int y = 0; y < GRID_COLUMNS; y++) {
            // MAKE DEFAULT CELL
            cell cell = create_cell();

            // ADD DYNAMIC DATA TO CELL (INDEX, SIZE, ...)
            int index = (x * GRID_COLUMNS) + y;
            cell.index = index;
            cell.start_x = GRID_PADDING_SIZE + (x * cell_size);
            cell.start_y = GRID_PADDING_SIZE + (y * cell_size);
            cell.size = cell_size;

            cells[index] = cell;

            // DRAW CELL BORDERS
            DrawBorder(renderer, cells, x, y);

            // DRAW CELL SQUARE
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE_FLOAT);
            DrawSquare(renderer, cells, x, y);
        }
    }
}

void DrawGrid(SDL_Renderer *renderer, cell* cells) {
    // DRAW BACKGROUND COLOR
    SetBackground(renderer);

    for (int x = 0; x < GRID_ROWS; x++) {
        for (int y = 0; y < GRID_COLUMNS; y++) {
            // DRAW CELL BORDERS
            DrawBorder(renderer, cells, x, y);

            // DRAW CELL SQUARE
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE_FLOAT);
            DrawSquare(renderer, cells, x, y);
        }
    }

    // UPDATE SCREEN RENDERING
    SDL_RenderPresent(renderer);
    return;
}

static void SetBackground(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);
}

static void DrawBorder(SDL_Renderer *renderer, cell* cells, int x, int y) {
    const int index = (x * GRID_COLUMNS) + y;

    // Top Edge
    const int top_bit = cells[index].borders & (1 << Up);
    const int top_edge_color = top_bit == 0 ? GRID_SQUARE_COLOR : GRID_BORDER_COLOR;
    const SDL_FRect top_edge = { 
        .x = top_bit == 0 ? cells[index].start_x + GRID_CELL_BORDER_SIZE : cells[index].start_x, 
        .y = cells[index].start_y, 
        .w = cells[index].size,
        .h = GRID_CELL_BORDER_SIZE 
    };
    SDL_SetRenderDrawColor(renderer, top_edge_color, top_edge_color, top_edge_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &top_edge);

    // Bottom Edge
    const int bottom_bit = cells[index].borders & (1 << Down);
    const int bottom_edge_color = bottom_bit == 0 ? GRID_SQUARE_COLOR : GRID_BORDER_COLOR;
    const SDL_FRect bottom_edge = { 
        .x = bottom_bit == 0 ? cells[index].start_x + GRID_CELL_BORDER_SIZE : cells[index].start_x,
        .y = cells[index].start_y + cells[index].size,
        .w = bottom_bit == 0 ? cells[index].size : cells[index].size + GRID_CELL_BORDER_SIZE,
        .h = GRID_CELL_BORDER_SIZE 
    };
    SDL_SetRenderDrawColor(renderer, bottom_edge_color, bottom_edge_color, bottom_edge_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &bottom_edge);

    // Left Edge
    const int left_bit = cells[index].borders & (1 << Left);
    const int left_edge_color = left_bit == 0 ? GRID_SQUARE_COLOR : GRID_BORDER_COLOR;
    const SDL_FRect left_edge = { 
        .x = cells[index].start_x,
        .y = left_bit == 0 ? cells[index].start_y + GRID_CELL_BORDER_SIZE : cells[index].start_y, 
        .w = GRID_CELL_BORDER_SIZE,
        .h = left_bit == 0 ? cells[index].size - GRID_CELL_BORDER_SIZE : cells[index].size,
    };
    SDL_SetRenderDrawColor(renderer, left_edge_color, left_edge_color, left_edge_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &left_edge);

    // Right Edge
    const int right_bit = cells[index].borders & (1 << Right);
    const int right_edge_color = right_bit == 0 ? GRID_SQUARE_COLOR : GRID_BORDER_COLOR;
    const SDL_FRect right_edge = { 
        .x = cells[index].start_x + cells[index].size, 
        .y = right_bit == 0 ? cells[index].start_y + GRID_CELL_BORDER_SIZE : cells[index].start_y, 
        .w = GRID_CELL_BORDER_SIZE, 
        .h = right_bit == 0 ? cells[index].size - GRID_CELL_BORDER_SIZE : cells[index].size,
    };
    SDL_SetRenderDrawColor(renderer, right_edge_color, right_edge_color, right_edge_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &right_edge);
}

static void DrawSquare(SDL_Renderer *renderer, cell* cells, int x, int y) {
    const int index = (x * GRID_COLUMNS) + y;
    const int square_size = cells[index].size - GRID_CELL_BORDER_SIZE;
    SDL_FRect inner_square = { 
        .x = cells[index].start_x + GRID_CELL_BORDER_SIZE, 
        .y = cells[index].start_y + GRID_CELL_BORDER_SIZE, 
        .w = square_size, .h = square_size
    };
    SDL_RenderFillRect(renderer, &inner_square);
}