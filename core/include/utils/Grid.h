#ifndef GRID_H
#define GRID_H

#include <SDL3/SDL.h>
#include <Cell.h>

#define GRID_ROWS 25
#define GRID_COLUMNS 25
#define GRID_ARRAY_SIZE (GRID_ROWS * GRID_COLUMNS)

#define GRID_CELL_SIZE 28
#define GRID_CELL_BORDER_SIZE 2

#define GRID_PADDING_SIZE 20

#define GRID_SQUARE_COLOR 50    // GRID SQUARE COLOR NUMBER: [0-255]
#define GRID_BORDER_COLOR 0     // GRID BORDER COLOR NUMBER: [0-255]

void CreateGrid(SDL_Renderer *renderer, cell* cells);

void DrawGrid(SDL_Renderer *renderer, cell* cells, bool show_path, bool show_gradient);

static void DrawBorder(SDL_Renderer *renderer, cell* cells, int x, int y);

static void DrawSquare(SDL_Renderer *renderer, cell* cells, int x, int y, bool show_path, bool show_gradient);

#endif