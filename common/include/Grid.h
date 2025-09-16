#ifndef GRID_H
#define GRID_H

#include <SDL3/SDL.h>
#include <Cell.h>

// CALCULATE THE GRID NUMBERS:
// (WINDOW_WIDTH - (GRID_PADDING_SIZE * 2)) / (GRID_CELL_SIZE + (GRID_CELL_BORDER_SIZE * 2))

static const int GRID_ROWS = 25;
static const int GRID_COLUMNS = 25;

static const int GRID_PADDING_SIZE = 20;

static const int GRID_CELL_SIZE = 28;
static const int GRID_CELL_BORDER_SIZE = 2;

void CreateGrid(SDL_Renderer *renderer, cell* cells, int window_width, int window_height);

void DrawMaze(SDL_Renderer *renderer, cell* cells);
void DrawBorder(SDL_Renderer *renderer, cell* cells, int x, int y);
void DrawSquare(SDL_Renderer *renderer, cell* cells, int x, int y);

int Walk(int remaining, cell* cells);
int GetRandomNeighbor(int current_index, int array_size, cell* cells);

bool contains(int element, int array_size, int* path);

#endif