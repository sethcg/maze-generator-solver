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

    pathContext->children = (int*) malloc(4 * sizeof(int));

    pathContext->openSet = (cell_node*) malloc(GRID_ARRAY_SIZE * sizeof(cell_node));
    pathContext->closedSet = (int*) malloc(GRID_ARRAY_SIZE * sizeof(int));

    // INITIALIZE EMPTY CELL_NODE ARRAYS
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        cell_node open_node = create_cell_node();
        pathContext->openSet[i] = open_node;

        pathContext->closedSet[i] = -1;
    }

    return AStarPathfinding(pathContext, cells);
}

PathContext* AStarPathfinding(PathContext* pathContext, cell* cells) {

    // ADD THE START NODE
    pathContext->openSet[0].cell_index = pathContext->start;
    pathContext->openSet[0].f = 0;

    pathContext->current_node = pathContext->openSet[0];

    int open_index = 1;
    int closed_index = 0;

    int test_open[GRID_ARRAY_SIZE];
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        test_open[i] = -1;
    }
    test_open[0] = pathContext->start;

    int test_closed[GRID_ARRAY_SIZE];

    while(open_index > 0) {
        // LET THE CURRENT NODE EQUAL THE NODE WITH THE LOWEST 'F' VALUE
        pathContext->current_node = pathContext->openSet[0];
        int remove_index = 0;
        for(int i = 0; i < open_index; i++) {
            int new_f = pathContext->openSet[i].f;
            int current_f = pathContext->current_node.f;
            if(new_f >= 0 && current_f >= 0 &&  new_f < current_f) {
                pathContext->current_node = pathContext->openSet[i];
                remove_index = i;
            }
        }
        int current_index = pathContext->current_node.cell_index;

        // REMOVE CURRENT NODE FROM THE OPEN SET
        for (int i = remove_index; i < open_index; ++i) {
            pathContext->openSet[i] = pathContext->openSet[i + 1];
            test_open[i] = test_open[i + 1];
        }
        open_index--;

        // ADD THE CURRENT NODE TO THE CLOSED SET
        pathContext->closedSet[closed_index] = current_index;
        test_closed[closed_index] = current_index;
        closed_index++;

        // IF DONE, EXIT
        if(current_index == pathContext->end) {
            break;
        }

        // GET ADJACENT 'CHILD' NODES
        int children_size = GetAdjacentNodes(pathContext, cells);
        for (int i = 0; i < children_size; i++) {
            int child_index = pathContext->children[i];
            if(ClosedSetContainsChild(pathContext, child_index)) {
                continue;
            }

            // CREATE 'F', 'G', 'H' VALUES
            cell_node temp_node = create_cell_node();
            temp_node.cell_index = child_index;
            temp_node.g = pathContext->current_node.g + 1;
            temp_node.h = GetHeuristicValue(child_index, pathContext->end);
            temp_node.f = temp_node.g + temp_node.h;

            // CHILD ALREADY IN THE OPEN SET
            int current_g_value = pathContext->openSet[current_index].g;
            bool test1 = OpenSetContainsChild(pathContext, child_index);
            bool test2 = temp_node.g > current_g_value;
            if(OpenSetContainsChild(pathContext, child_index)) {
                if(temp_node.g > pathContext->openSet[current_index].g) {
                    continue;
                }
            }

            // ADD CHILD NODE TO OPEN SET
            pathContext->openSet[open_index] = temp_node;
            test_open[open_index] = temp_node.cell_index;
            open_index++;
        }
    }

    return pathContext;
}

static int GetHeuristicValue(int index, int end_index) {
    int current_x = (index / GRID_ROWS) + 1;
    int current_y = (index % GRID_COLUMNS) + 1;

    // MANHATTAN DISTANCE BETWEEN TWO POINTS
    return abs(current_x - GRID_ROWS) + abs(current_y - GRID_COLUMNS);
}

static bool ClosedSetContainsChild(PathContext* pathContext, int child_index) {
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        if(pathContext->closedSet[i] == child_index) {
            return true;
        }
    }
    return false;
}

static bool OpenSetContainsChild(PathContext* pathContext, int child_index) {
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        if(pathContext->openSet[i].cell_index == child_index) {
            return true;
        }
    }
    return false;
}

static int GetAdjacentNodes(PathContext* pathContext, cell* cells) {
    int array_size = 0;
    int cell_index = pathContext->current_node.cell_index;
    memset(pathContext->children, 0, 4 * sizeof (int));
    
    // TOP EDGE
    if(!(cells[cell_index].borders & (1 << Up))) {
        pathContext->children[array_size] = (cell_index - 1);
        array_size++;
    }

    // BOTTOM EDGE
    if(!(cells[cell_index].borders & (1 << Down))) {
        pathContext->children[array_size] = (cell_index + 1);
        array_size++;
    }

    // LEFT EDGE
    if(!(cells[cell_index].borders & (1 << Left))) {
        pathContext->children[array_size] = (cell_index - GRID_ROWS);
        array_size++;
    }

    // RIGHT EDGE
    if(!(cells[cell_index].borders & (1 << Right))) {
        pathContext->children[array_size] = (cell_index + GRID_ROWS);
        array_size++;
    }

    return array_size;
}