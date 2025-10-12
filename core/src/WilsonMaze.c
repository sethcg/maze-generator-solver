// WILSON'S ALGORITHM MAZE GENERATION

#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <Grid.h>
#include <Cell.h>
#include <WilsonMaze.h>

MazeContext* Init_WilsonMaze(cell* cells) {
    MazeContext* mazeContext = (MazeContext*) malloc(sizeof(MazeContext));
    mazeContext->unvisited = (int*) malloc(GRID_ARRAY_SIZE * sizeof(int));
    mazeContext->visited = (cell_direction*) malloc(GRID_ARRAY_SIZE * sizeof(cell_direction));
    mazeContext->neighbors = (int*) malloc(4 * sizeof(int));

    // INITIALIZE GRID, WITH ALL BORDERS AND UNVISITED
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        cells[i].path_cell = false;
        cells[i].visited = false;
        cells[i].borders = (uint8_t) 0b1111;
    }

    // CHOOSE A RANDOM STARTING CELL, MARK VISITED
    cells[rand() % (GRID_ROWS * GRID_COLUMNS)].visited = true;

    // RANDOMLY WALK WITH LOOP-ERASING
    mazeContext->remaining = (GRID_ARRAY_SIZE - 1);

    return mazeContext;
}

bool Continue_WilsonMaze(MazeContext* mazeContext, cell* cells) {
    Walk(mazeContext, cells);
    return (mazeContext->remaining > 0);
}

static void Walk(MazeContext* mazeContext, cell* cells) {
    // INITIALIZE ARRAYS
    int index = 0;
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        mazeContext->visited[i].cell_index = -1;
        mazeContext->visited[i].dir = None;

        if(cells[i].visited) {
            mazeContext->unvisited[i] = -1;
        } else {
            mazeContext->unvisited[index] = cells[i].index;
            index++;
        }
    }

    // PICK RANDOM UNVISITED CELL TO START FROM
    mazeContext->current_index = mazeContext->unvisited[rand() % (mazeContext->remaining)];

    int running_index = 0;
    while(!cells[mazeContext->current_index].visited) {
        mazeContext->visited[running_index].cell_index = mazeContext->current_index;

        // SELECT A RANDOM NEIGHBOR TO WALK TOWARDS
        GetRandomNeighbor(mazeContext, cells);

        // IF THE PATH CONTAINS LOOP, ERASE THE LOOP
        if(ContainsLoop(mazeContext)) {
            for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
                if(mazeContext->visited[i].cell_index == mazeContext->next_index) {
                    running_index = i;
                    break;
                }
            }
            for(int i = running_index + 1; i < GRID_ARRAY_SIZE; i++) {
                mazeContext->visited[i].cell_index = -1;
                mazeContext->visited[i].dir = None;
            }
        } else {
            // ADD DIRECTION FROM CURRENT TO NEXT
            mazeContext->visited[running_index].dir = 
                mazeContext->next_index == (mazeContext->current_index + 1) ? Down :
                mazeContext->next_index == (mazeContext->current_index - 1) ? Up :
                mazeContext->next_index == (mazeContext->current_index - GRID_ROWS) ? Left :
                mazeContext->next_index == (mazeContext->current_index + GRID_ROWS) ? Right : None;
            running_index++;
        }

        mazeContext->current_index = mazeContext->next_index;
    }

    // APPLY THE PATH
    for(int i = 0; i < running_index; i++) {
        direction dir = mazeContext->visited[i].dir;

        int current = mazeContext->visited[i].cell_index;
        int next = 
            dir == Up ? (current - 1) :
            dir == Down ? (current + 1) :
            dir == Right ? (current + GRID_ROWS) :
            dir == Left ? (current - GRID_ROWS) : -1;

        // MARK CURRENT AS VISITED
        cells[current].visited = true;

        // APPLY BORDER CHANGES TO CURRENT CELL
        cells[current].borders = dir == None 
            ? cells[current].borders
            : cells[current].borders ^ (1 << ((int) dir));

        // APPLY OPPOSITE BORDER CHANGES TO THE NEXT CELL
        if(cells[index].borders & (1 << ((int) dir)) == 0) {
            cells[next].borders = 
                dir == Up ? cells[next].borders | (1 << Down) :
                dir == Down ? cells[next].borders | (1 << Up) :
                dir == Right ? cells[next].borders | (1 << Left) :
                dir == Left ? cells[next].borders | (1 << Right) : cells[next].borders;
        } else {
            cells[next].borders = 
                dir == Up ? cells[next].borders & ~(1 << Down) :
                dir == Down ? cells[next].borders & ~(1 << Up) :
                dir == Right ? cells[next].borders & ~(1 << Left) :
                dir == Left ? cells[next].borders & ~(1 << Right) : cells[next].borders;
        }

        current = next;
        mazeContext->remaining = mazeContext->remaining - 1;
    }
}

static void GetRandomNeighbor(MazeContext* mazeContext, cell* cells) {
    int array_size = 0;
    memset(mazeContext->neighbors, 0, 4 * sizeof (int));

    // TOP EDGE
    if(mazeContext->current_index % GRID_ROWS != 0) {
        mazeContext->neighbors[array_size] = (mazeContext->current_index - 1);
        array_size++;
    }

    // BOTTOM EDGE
    if((mazeContext->current_index + 1) % GRID_ROWS != 0) {
        mazeContext->neighbors[array_size] = (mazeContext->current_index + 1);
        array_size++;
    }

    // LEFT EDGE
    if(mazeContext->current_index > GRID_ROWS) {
        mazeContext->neighbors[array_size] = (mazeContext->current_index - GRID_ROWS);
        array_size++;
    }

    // RIGHT EDGE
    if((mazeContext->current_index + GRID_ROWS) < GRID_ARRAY_SIZE) {
        mazeContext->neighbors[array_size] = (mazeContext->current_index + GRID_ROWS);
        array_size++;
    }

    // PICK RANDOM DIRECTION, FROM AVAILABLE
    mazeContext->next_index = mazeContext->neighbors[rand() % (array_size)];
}

static bool ContainsLoop(MazeContext* mazeContext) {
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        if(mazeContext->visited[i].cell_index == mazeContext->next_index) {
            return true;
        }
    }
    return false;
}
