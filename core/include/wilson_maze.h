#ifndef MAZE_H
#define MAZE_H

#include <SDL3/SDL.h>
#include <cell.h>

typedef struct {
    int cell_index;
    direction dir;
} cell_direction;

void DrawMaze(SDL_Renderer *renderer, cell* cells);

static void Walk(int* remaining, int* unvisited, cell_direction* visited, cell* cells);

static void GetRandomNeighbor(int current_index, int* next_index, int* neighbors, cell* cells);

static bool contains(int value, int array_size, cell_direction* visited);

#endif