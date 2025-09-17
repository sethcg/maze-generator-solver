// WILSON'S ALGORITHM MAZE GENERATION

#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <Grid.h>
#include <Cell.h>
#include <WilsonMaze.h>

void Draw_WilsonMaze(SDL_Renderer *renderer, cell* cells) {
    int* remaining = malloc(sizeof(int));
    int* unvisited = malloc(GRID_ARRAY_SIZE* sizeof(int));
    cell_direction* visited = malloc(GRID_ARRAY_SIZE * sizeof(cell_direction));

    // INITIALIZE GRID, WITH ALL BORDERS AND UNVISITED
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        cells[i].visited = false;
        cells[i].borders = (uint8_t) 0b1111;
    }

    // CHOOSE A RANDOM STARTING CELL, MARK VISITED
    cells[rand() % (GRID_ROWS * GRID_COLUMNS)].visited = true;

    // RANDOMLY WALK WITH LOOP-ERASING
    *remaining = (int) (GRID_ARRAY_SIZE - 1);
    while (*remaining > 0) {
        Walk(remaining, unvisited, visited, cells);

        // DRAW GRID EACH ITERATION OF WALK
        DrawGrid(renderer, cells);
    }

    free(remaining);
    free(unvisited);
    free(visited);
}

static void Walk(int* remaining, int* unvisited, cell_direction* visited, cell* cells) {
    int* neighbors = malloc(4 * sizeof(int));
    int* next_index = malloc(sizeof(int));

    // INITIALIZE ARRAYS
    int index = 0;
    for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
        visited[i].cell_index = -1;
        visited[i].dir = None;

        if(cells[i].visited) {
            unvisited[i] = -1;
        } else {
            unvisited[index] = cells[i].index;
            index++;
        }
    }

    // PICK RANDOM UNVISITED CELL TO START FROM
    int current_index = unvisited[rand() % (*remaining)];

    int running_index = 0;
    while(!cells[current_index].visited) {
        visited[running_index].cell_index = current_index;

        // SELECT A RANDOM NEIGHBOR TO WALK TOWARDS
        GetRandomNeighbor(current_index, next_index, neighbors, cells);

        if(contains(*next_index, GRID_ARRAY_SIZE, visited)) {
            for(int i = 0; i < GRID_ARRAY_SIZE; i++) {
                if(visited[i].cell_index == *next_index) {
                    running_index = i;
                    break;
                }
            }
            for(int i = running_index + 1; i < GRID_ARRAY_SIZE; i++) {
                visited[i].cell_index = -1;
                visited[i].dir = None;
            }
        } else {
            // ADD DIRECTION FROM CURRENT TO NEXT
            visited[running_index].dir = 
                *next_index == (current_index + 1) ? Down :
                *next_index == (current_index - 1) ? Up :
                *next_index == (current_index - GRID_ROWS) ? Left :
                *next_index == (current_index + GRID_ROWS) ? Right : None;
            running_index++;
        }

        current_index = *next_index;
    }

    // APPLY THE PATH
    for(int i = 0; i < running_index; i++) {
        direction dir = visited[i].dir;

        int current = visited[i].cell_index;
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
        *remaining = *remaining - 1;
    }

    free(neighbors);
    free(next_index);
}

static void GetRandomNeighbor(int current_index, int* next_index, int* neighbors, cell* cells) {
    memset(neighbors, 0, 4 * sizeof (int));
    int neighbors_num = 0;

    // TOP EDGE
    if(current_index % GRID_ROWS != 0) {
        neighbors[neighbors_num] = (current_index - 1);
        neighbors_num++;
    }

    // BOTTOM EDGE
    if((current_index + 1) % GRID_ROWS != 0) {
        neighbors[neighbors_num] = (current_index + 1);
        neighbors_num++;
    }

    // LEFT EDGE
    if(current_index > GRID_ROWS) {
        neighbors[neighbors_num] = (current_index - GRID_ROWS);
        neighbors_num++;
    }

    // RIGHT EDGE
    if((current_index + GRID_ROWS) < GRID_ARRAY_SIZE) {
        neighbors[neighbors_num] = (current_index + GRID_ROWS);
        neighbors_num++;
    }

    // PICK RANDOM DIRECTION, FROM AVAILABLE
    *next_index = neighbors[rand() % (neighbors_num)];
}

static bool contains(int value, int array_size, cell_direction* visited) {
    for(int i = 0; i < array_size; i++) {
        if(visited[i].cell_index == value) {
            return true;
        }
    }
    return false;
}

