#pragma once

// CALCULATE THE GRID NUMBERS:
// (WINDOW_WIDTH - (GRID_PADDING_SIZE * 2)) / (GRID_CELL_SIZE + (GRID_CELL_BORDER_SIZE * 2))

const int GRID_ROWS = 25;
const int GRID_COLUMNS = 25;

const int GRID_PADDING_SIZE = 20;

const int GRID_CELL_SIZE = 28;
const int GRID_CELL_BORDER_SIZE = 2;

typedef enum {
    North,
    South,
    East,
    West
} direction;

typedef struct {
    int start_x;    // TOP-LEFT X COORDINATE
    int start_y;    // TOP-LEFT Y COORDINATE
    int size;
    bool visited;
} cell;

void DrawBorder(SDL_Renderer *renderer, cell* cells, int x, int y, direction dir);
void DrawSquare(SDL_Renderer *renderer, cell* cells, int x, int y);
