#ifndef ASTAR_PATHFINDING_H
#define ASTAR_PATHFINDING_H

#include <SDL3/SDL.h>
#include <Cell.h>

typedef struct {
    int cell_index;
    int f;  // TOTAL COST OF THE NODE
    int g;  // DISTANCE BETWEEN THE CURRENT NODE AND THE START NODE
    int h;  // HEURISTIC - ESTIMATED DISTANCE FROM THE CURRENT NODE TO THE END NODE
} cell_node;

typedef struct PathContext {
    int start;
    int end;
    int* children;
    cell_node current_node;
    cell_node* openSet;
    int* closedSet;
} PathContext;

#define create_cell_node() { .cell_index = -1, .f = 0, .g = 0, .h = 0 };

PathContext* Init_AStarPathfinding(int start, int end, cell* cells);

PathContext* AStarPathfinding(PathContext* pathContext, cell* cells);

static int GetHeuristicValue(int index, int end_index);

static bool ClosedSetContainsChild(PathContext* pathContext, int child_index);

static bool OpenSetContainsChild(PathContext* pathContext, int child_index);

static int GetAdjacentNodes(PathContext* pathContext, cell* cells);

#endif