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

    pathContext->children = (cell_direction*) malloc(4 * sizeof(cell_direction));
    pathContext->openSet = (cell_node*) malloc(GRID_ARRAY_SIZE * sizeof(cell_node));

    pathContext->closedSet = (path_node*) malloc(GRID_ARRAY_SIZE * sizeof(path_node));

    // INITIALIZE EMPTY CELL_NODE ARRAYS
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        cell_node open_node = create_cell_node();
        pathContext->openSet[i] = open_node;

        path_node closed_node = create_path_node();
        pathContext->closedSet[i] = closed_node;
    }

    return AStarPathfinding(pathContext, cells);
}

PathContext* AStarPathfinding(PathContext* pathContext, cell* cells) {

    // ADD THE START NODE
    pathContext->openSet[0].cell_index = pathContext->start;
    pathContext->openSet[0].g_score = 0;
    pathContext->openSet[0].h_score = 0;
    pathContext->openSet[0].f_score = 0;

    pathContext->current_node = pathContext->openSet[0];

    int open_index = 1;
    int closed_index = 0;

    while(open_index > 0) {
        // LET THE CURRENT NODE EQUAL THE NODE WITH THE LOWEST 'F' VALUE
        pathContext->current_node = pathContext->openSet[0];
        int remove_index = 0;
        for(int i = 0; i < open_index; i++) {
            if(pathContext->openSet[i].f_score < pathContext->current_node.f_score) {
                pathContext->current_node = pathContext->openSet[i];
                remove_index = i;
            }
        }

        // REMOVE CURRENT NODE FROM THE OPEN SET
        for (int i = remove_index; i < open_index; ++i) {
            pathContext->openSet[i] = pathContext->openSet[i + 1];
        }
        open_index--;

        // ADD THE CURRENT NODE TO THE CLOSED SET
        // SET THE DIRECTION TO THE OPPOSITE, FOR BACKTRACKING PURPOSES
        pathContext->closedSet[closed_index].dir = closed_index == pathContext->start ? None 
            : pathContext->current_node.dir == Up ? Down
            : pathContext->current_node.dir == Down ? Up
            : pathContext->current_node.dir == Right ? Left
            : pathContext->current_node.dir == Left ? Right
            : None;
        pathContext->closedSet[closed_index].cell_index = pathContext->current_node.cell_index;
        pathContext->closedSet[closed_index].g_score = pathContext->current_node.g_score;
        closed_index++;

        // IF CURRENT INDEX EQUALS END INDEX, EXIT
        if(pathContext->current_node.cell_index == pathContext->end) {
            break;
        }

        // GET ADJACENT 'CHILD' NODES
        int children_size = GetAdjacentNodes(pathContext, cells);
        for (int i = 0; i < children_size; i++) {
            int child_index = pathContext->children[i].cell_index;
            if(ClosedSetContainsChild(pathContext, child_index)) {
                continue;
            }

            // CREATE 'F', 'G', 'H' VALUES
            cell_node temp_node = create_cell_node();
            temp_node.cell_index = child_index;
            temp_node.dir = pathContext->children[i].dir;
            temp_node.g_score = pathContext->current_node.g_score + 1;
            temp_node.h_score = GetHeuristicValue(child_index, pathContext->end);
            temp_node.f_score = temp_node.g_score + temp_node.h_score;

            // CHILD ALREADY IN THE OPEN SET
            if(OpenSetContainsChild(pathContext, child_index)) {
                if(temp_node.g_score > pathContext->current_node.g_score) {
                    continue;
                }
            }

            // ADD CHILD NODE TO OPEN SET
            pathContext->openSet[open_index] = temp_node;
            open_index++;
        }
    }

    return BacktrackPath(pathContext, cells);
}

PathContext* BacktrackPath(PathContext* pathContext, cell* cells) {
    // GET THE POSITION OF THE END INDEX
    int path_size = 0;
    int current_position = -1;
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        if(pathContext->closedSet[i].cell_index == pathContext->end) {
            current_position = i;
            break;
        }
    }
    cells[pathContext->end].path_cell = true;
    cells[pathContext->start].path_cell = true;

    // BACKTRACK THROUGH THE INDICES/DIRECTIONS
    while(current_position > 0) {
        direction current_dir = pathContext->closedSet[current_position].dir;
        int current_index = pathContext->closedSet[current_position].cell_index;   
        
        int next_index = 
            current_dir == Up ? current_index - 1 :
            current_dir == Down ? current_index + 1 :
            current_dir == Left ? current_index - GRID_ROWS :
            current_dir == Right ? current_index + GRID_ROWS :
            None;

        // RECALCULATE THE CURRENT ARRAY POSITION
        for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
            if(pathContext->closedSet[i].cell_index == next_index) {
                current_position = i;
                break;
            }
        }

        // FLAG THE CELL AS BEING PART OF THE PATH
        cells[next_index].path_cell = true;
        path_size++;
    }

    // ADD GRADIENT TO EACH PATH STEP
    rgb_color start_color = create_rgb_color(255, 0, 0);
    rgb_color end_color = create_rgb_color(0, 255, 0);

    cells[pathContext->start].path_gradient = start_color;
    cells[pathContext->end].path_gradient = end_color;

    float step_r = (float)(end_color.r - start_color.r) / (float) path_size;
    float step_g = (float)(end_color.g - start_color.g) / (float) path_size;
    float step_b = (float)(end_color.b - start_color.b) / (float) path_size;

    int path_position = 0;
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        int current_index = pathContext->closedSet[i].cell_index;
        if(cells[current_index].path_cell) {
            // ADD THE PATH GRADIENT COLOR TO THE CELL
            int r = start_color.r + (path_position * step_r); 
            int g = start_color.g + (path_position * step_g);
            int b = start_color.b + (path_position * step_b);
            rgb_color current_color = create_rgb_color(r, g, b);
            cells[current_index].path_gradient = current_color;
            path_position++;
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
        if(pathContext->closedSet[i].cell_index == child_index) {
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
        pathContext->children[array_size].cell_index = (cell_index - 1);
        pathContext->children[array_size].dir = Up;
        array_size++;
    }

    // BOTTOM EDGE
    if(!(cells[cell_index].borders & (1 << Down))) {
        pathContext->children[array_size].cell_index = (cell_index + 1);
        pathContext->children[array_size].dir = Down;
        array_size++;
    }

    // LEFT EDGE
    if(!(cells[cell_index].borders & (1 << Left))) {
        pathContext->children[array_size].cell_index = (cell_index - GRID_ROWS);
        pathContext->children[array_size].dir = Left;
        array_size++;
    }

    // RIGHT EDGE
    if(!(cells[cell_index].borders & (1 << Right))) {
        pathContext->children[array_size].cell_index = (cell_index + GRID_ROWS);
        pathContext->children[array_size].dir = Right;
        array_size++;
    }

    return array_size;
}