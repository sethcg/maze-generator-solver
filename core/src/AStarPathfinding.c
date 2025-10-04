// A* ALGORITHM MAZE PATHFINDING

#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <Grid.h>
#include <Cell.h>
#include <AStarPathfinding.h>

PathContext* Init_AStarPathfinding(int start, int end, cell* cells) {
    PathContext* pathContext = (PathContext*) malloc(sizeof(PathContext));
    pathContext->start = start;
    pathContext->end = end;

    pathContext->openSet = (cell_node*) malloc(GRID_ARRAY_SIZE * sizeof(cell_node));
    pathContext->closedSet = (cell_node*) malloc(GRID_ARRAY_SIZE * sizeof(cell_node));

    // INITIALIZE EMPTY CELL_NODE ARRAYS
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        cell_node open_node = create_cell_node();
        pathContext->openSet[i] = open_node;

        cell_node closed_node = create_cell_node();
        pathContext->closedSet[i] = closed_node;
    }

    // ADD THE START NODE
    pathContext->openSet[0].cell_index = cells[start].index;
    pathContext->openSet[0].f = 0;
    pathContext->openSet[0].g = 0;
    pathContext->openSet[0].h = 0;

    // while(openSet not empty) {

    // }

    return pathContext;
}