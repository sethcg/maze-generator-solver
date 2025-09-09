#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
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

            // SET ALL BORDER BITS TO "TRUE"
            cells[(x * GRID_COLUMNS) + y].borders = 0b1111;

            // ADD BORDERS
            DrawBorder(renderer, cells, x, y);

            // ADD SQUARE
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE_FLOAT);
            DrawSquare(renderer, cells, x, y);
        }
    }

    return cells;
}

void DrawGrid(SDL_Renderer *renderer, cell* cells) {
    for (int x = 0; x < GRID_ROWS; x++) {
        for (int y = 0; y < GRID_COLUMNS; y++) {

            // ADD BORDERS
            DrawBorder(renderer, cells, x, y);

            // ADD SQUARE
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE_FLOAT);
            DrawSquare(renderer, cells, x, y);
        }
    }

    return;
}

void DrawMaze(SDL_Renderer *renderer, cell* cells) {
    int remaining = GRID_ROWS * GRID_COLUMNS;

    while (remaining > 0) {

        Walk(cells);
        DrawGrid(renderer, cells);

        remaining = 0;
    }
}

void Walk(cell* cells) {
    srand(time(NULL));
    const int start_index = rand() % ((GRID_ROWS * GRID_COLUMNS) + 1);
    cells[start_index].visited = true;

    // CLEAR ALL BORDER BITS (REMOVE BORDERS)
    ToggleBorder(start_index, cells, North);
    ToggleBorder(start_index, cells, South);
    ToggleBorder(start_index, cells, East);
    ToggleBorder(start_index, cells, West);

}

void ToggleBorder(int index, cell* cells, direction dir) {
    const int arraySize = GRID_ROWS * GRID_COLUMNS;
    int neighbor_index;
    switch(dir) {
        case North:
            cells[index].borders = cells[index].borders ^ (1 << 0);
            neighbor_index = (index - 1);
            if(neighbor_index >= 0 && neighbor_index < arraySize && index % GRID_ROWS != 0) {
                cells[neighbor_index].borders = cells[index].borders & (1 << 0) == 0 
                    ? cells[neighbor_index].borders | (1 << 1)
                    : cells[neighbor_index].borders & ~(1 << 1);
            }
            break;
        case South:
            cells[index].borders = cells[index].borders ^ (1 << 1);
            neighbor_index = (index + 1);
            if(neighbor_index < arraySize && neighbor_index % GRID_ROWS != 0) {
                cells[neighbor_index].borders = cells[index].borders & (1 << 1) != 0 
                    ? cells[neighbor_index].borders | (1 << 0)
                    : cells[neighbor_index].borders & ~(1 << 0);
            }
            break;
        case East:
            cells[index].borders = cells[index].borders ^ (1 << 2);
            neighbor_index = (index + GRID_ROWS);
            if(neighbor_index < arraySize) {
                cells[neighbor_index].borders = cells[index].borders & (1 << 2) != 0 
                    ? cells[neighbor_index].borders | (1 << 3)
                    : cells[neighbor_index].borders & ~(1 << 3);
            }
            break;
        case West:
            cells[index].borders = cells[index].borders ^ (1 << 3);
            neighbor_index = (index + GRID_ROWS);
            if(neighbor_index >= 0 && neighbor_index < arraySize) {
                cells[neighbor_index].borders = cells[index].borders & (1 << 3) == 0 
                    ? cells[neighbor_index].borders | (1 << 2)
                    : cells[neighbor_index].borders & ~(1 << 2);
            }
            break;
    }
}

void DrawBorder(SDL_Renderer *renderer, cell* cells, int x, int y) {
    const int grid_color = 50;
    const int cell_size = cells[(x * GRID_COLUMNS) + y].size;
    const int borders = cells[(x * GRID_COLUMNS) + y].borders;
    const int start_x = cells[(x * GRID_COLUMNS) + y].start_x;
    const int start_y = cells[(x * GRID_COLUMNS) + y].start_y;

    // NORTH
    const int n_bit = borders & (1 << 0);
    const int n_color = n_bit == 0 ? grid_color : 0;
    const SDL_FRect north = { 
        .x = n_bit == 0 ? start_x + GRID_CELL_BORDER_SIZE : start_x, 
        .y = start_y, 
        .w = cell_size,
        .h = GRID_CELL_BORDER_SIZE 
    };
    SDL_SetRenderDrawColor(renderer, n_color, n_color, n_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &north);

    // SOUTH
    const int s_bit = borders & (1 << 1);
    const int s_color = s_bit == 0 ? grid_color : 0;
    const SDL_FRect south = { 
        .x = s_bit == 0 ? start_x + GRID_CELL_BORDER_SIZE : start_x,
        .y = start_y + cell_size,
        .w = cell_size,
        .h = GRID_CELL_BORDER_SIZE 
    };
    SDL_SetRenderDrawColor(renderer, s_color, s_color, s_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &south);

    // EAST
    const int e_bit = borders & (1 << 2);
    const int e_color = e_bit == 0 ? grid_color : 0;
    const SDL_FRect east = { 
        .x = start_x,
        .y = e_bit == 0 ? start_y + GRID_CELL_BORDER_SIZE : start_y, 
        .w = GRID_CELL_BORDER_SIZE,
        .h = cell_size,
    };
    SDL_SetRenderDrawColor(renderer, e_color, e_color, e_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &east);

    // WEST
    const int w_bit = borders & (1 << 3);
    const int w_color = w_bit == 0 ? grid_color : 0;
    const SDL_FRect west = { 
        .x = start_x + cell_size, 
        .y = w_bit == 0 ? start_y + GRID_CELL_BORDER_SIZE : start_y, 
        .w = GRID_CELL_BORDER_SIZE, 
        .h = cell_size,
    };
    SDL_SetRenderDrawColor(renderer, w_color, w_color, w_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &west);
}

void DrawSquare(SDL_Renderer *renderer, cell* cells, int x, int y) {
    const int square_size = cells[(x * GRID_COLUMNS) + y].size - GRID_CELL_BORDER_SIZE;
    SDL_FRect inner_square = { 
        .x = cells[(x * GRID_COLUMNS) + y].start_x + GRID_CELL_BORDER_SIZE, 
        .y = cells[(x * GRID_COLUMNS) + y].start_y + GRID_CELL_BORDER_SIZE, 
        .w = square_size, .h = square_size
    };
    SDL_RenderFillRect(renderer, &inner_square);
}