#ifndef ASTAR_PATHFINDING_H
#define ASTAR_PATHFINDING_H

#include <SDL3/SDL.h>
#include <Cell.h>

typedef struct {
    direction dir;
    int cell_index;
    int f_score;  // TOTAL COST OF THE NODE
    int g_score;  // DISTANCE BETWEEN THE CURRENT NODE AND THE START NODE
    int h_score;  // HEURISTIC - ESTIMATED DISTANCE FROM THE CURRENT NODE TO THE END NODE
} cell_node;

typedef struct {
    direction dir;
    int cell_index;
    int g_score;
} path_node;

typedef struct PathContext {
    int start;
    int end;
    cell_direction* children;
    cell_node current_node;
    cell_node* openSet;  
    path_node* closedSet;
} PathContext;

#define create_path_node() { .cell_index = -1, .g_score = 0 };

#define create_cell_node() { .cell_index = -1, .f_score = 0, .g_score = 0, .h_score = 0 };

PathContext* Init_AStarPathfinding(int start, int end, cell* cells);

PathContext* AStarPathfinding(PathContext* pathContext, cell* cells);

PathContext* BacktrackPath(PathContext* pathContext, cell* cells);

static int GetHeuristicValue(int index, int end_index);

static bool ClosedSetContainsChild(PathContext* pathContext, int child_index);

static bool OpenSetContainsChild(PathContext* pathContext, int child_index);

static int GetAdjacentNodes(PathContext* pathContext, cell* cells);

#endif