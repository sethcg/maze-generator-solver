#ifndef WILSON_MAZE_H
#define WILSON_MAZE_H

#include <SDL3/SDL.h>
#include <Cell.h>

typedef struct MazeContext {
    int remaining;
    int current_index;
    int next_index;
    int* neighbors;
    int* unvisited;
    cell_direction* visited;
} MazeContext;

MazeContext* Init_WilsonMaze(cell* cells);

bool Continue_WilsonMaze(MazeContext* mazeContext, cell* cells);

static void Walk(MazeContext* mazeContext, cell* cells);

static void GetRandomNeighbor(MazeContext* mazeContext, cell* cells);

static bool ContainsLoop(MazeContext* mazeContext);

#endif