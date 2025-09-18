#ifndef MAZE_H
#define MAZE_H

#include <SDL3/SDL.h>
#include <Cell.h>

typedef struct {
    int cell_index;
    direction dir;
} cell_direction;

void Allocate_WilsonMaze(int* remaining, cell* cells);

bool Iterate_WilsonMaze(int* remaining, SDL_Renderer* renderer, cell* cells);

void Free_WilsonMaze();

static void Walk(int* remaining, cell* cells);

static void GetRandomNeighbor(int current_index, int* next_index, int* neighbors, cell* cells);

static bool contains(int value, int array_size);

#endif