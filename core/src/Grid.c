#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <Cell.h>
#include <Grid.h>
#include <SDL3/SDL.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

void SetBackground(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);
}

void CreateGrid(SDL_Renderer *renderer, cell* cells, int window_width, int window_height) {
    const int total_size = GRID_CELL_SIZE + (GRID_CELL_BORDER_SIZE * 2);
    for (int x = 0; x < GRID_ROWS; x++) {
        for (int y = 0; y < GRID_COLUMNS; y++) {
            // MAKE DEFAULT CELL
            cell cell = create_cell();

            // ADD DYNAMIC DATA TO CELL (INDEX, SIZE, ...)
            int index = (x * GRID_COLUMNS) + y;
            cell.index = (x * GRID_COLUMNS) + y;
            cell.start_x = GRID_PADDING_SIZE + (x * total_size);
            cell.start_y = GRID_PADDING_SIZE + (y * total_size);
            cell.size = total_size;

            cells[index] = cell;

            // DRAW CELL BORDERS AT (X, Y) COORDINATES
            DrawBorder(renderer, cells, x, y);

            // DRAW CELL SQUARE AT (X, Y) COORDINATES
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE_FLOAT);
            DrawSquare(renderer, cells, x, y);
        }
    }
}

void DrawGrid(SDL_Renderer *renderer, cell* cells) {
    // DRAW BACKGROUND COLOR
    SetBackground(renderer);

    for (int x = 0; x < GRID_ROWS; x++) {
        for (int y = 0; y < GRID_COLUMNS; y++) {
            // DRAW CELL BORDERS AT (X, Y) COORDINATES
            DrawBorder(renderer, cells, x, y);

            // DRAW CELL SQUARE AT (X, Y) COORDINATES
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE_FLOAT);
            DrawSquare(renderer, cells, x, y);
        }
    }

    // UPDATE SCREEN RENDERING
    SDL_RenderPresent(renderer);
    return;
}

void DrawMaze(SDL_Renderer *renderer, cell* cells) {
    const int array_size_max = GRID_ROWS * GRID_COLUMNS;

    // INITIALIZE GRID, WITH ALL BORDERS AND UNVISITED
    for(int i = 0; i < array_size_max; i++) {
        cells[i].visited = false;
        cells[i].borders = 0b1111;
    }

    // CHOOSE A RANDOM STARTING CELL, MARK VISITED
    int start_index = rand() % (GRID_ROWS * GRID_COLUMNS);
    cells[start_index].visited = true;

    // RANDOMLY WALK WITH LOOP-ERASING
    int remaining = array_size_max - 1;
    while (remaining > 0) {
        remaining = Walk(remaining, cells);
        DrawGrid(renderer, cells);
    }
    DrawGrid(renderer, cells);
}

int Walk(int remaining, cell* cells) {
    const int array_size_max = GRID_ROWS * GRID_COLUMNS;

    int index = 0;
    int unvisited_indices[array_size_max];
    for(int i = 0; i < array_size_max; i++) {
        unvisited_indices[i] = -1;
    }
    for(int i = 0; i < array_size_max; i++) {
        int cell_index = cells[i].index;
        bool cell_visited = cells[i].visited;
        if(cell_visited == false) {
            unvisited_indices[index] = cell_index;
            index++;
        }
    }

    // PICK RANDOM UNVISITED CELL TO START FROM
    int current_index = unvisited_indices[rand() % (remaining)];

    int indices[array_size_max];
    direction directions[array_size_max];
    for (int i = 0; i < array_size_max; i++) {
        indices[i] = -1;
        directions[i] = None;
    }

    int running_index = 0;
    while(!cells[current_index].visited) {
        indices[running_index] = current_index;

        // SELECT A RANDOM NEIGHBOR TO WALK TOWARDS
        int next_index = GetRandomNeighbor(current_index, array_size_max, cells);

        if(contains(next_index, array_size_max, indices)) {
            for(int i = 0; i < array_size_max; i++) {
                if(indices[i] == next_index) {
                    running_index = i;
                    break;
                }
            }
            for(int i = running_index + 1; i < array_size_max; i++) {
                indices[i] = -1;
                directions[i] = None;
            }
        } else {
            // ADD DIRECTION FROM CURRENT TO NEXT
            directions[running_index] = 
                next_index == (current_index + 1) ? Down :
                next_index == (current_index - 1) ? Up :
                next_index == (current_index - GRID_ROWS) ? Left :
                next_index == (current_index + GRID_ROWS) ? Right : None;
            running_index++;
        }

        current_index = next_index;
    }

    // APPLY THE PATH
    for(int i = 0; i < running_index; i++) {
        direction dir = directions[i];

        int current = indices[i];
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
        remaining--;
    }

    return remaining;
}

bool contains(int element, int array_size, int* path) {
    for(int i = 0; i < array_size; i++) {
        if(path[i] == element) {
            return true;
        }
    }
    return false;
}

int GetRandomNeighbor(int current_index, int array_size, cell* cells) {
    int neighbor_indices[4];  
    int neighbor_num = 0;

    bool top_edge = current_index % GRID_ROWS == 0;
    bool bottom_edge = (current_index + 1) % GRID_ROWS == 0;
    bool left_edge = current_index < GRID_ROWS;
    bool right_edge = (current_index + GRID_ROWS) >= array_size;

    if(!top_edge) {
        neighbor_indices[neighbor_num] = (current_index - 1);
        neighbor_num++;
    }
    if(!bottom_edge) {
        neighbor_indices[neighbor_num] = (current_index + 1);
        neighbor_num++;
    }
    if(!left_edge) {
        neighbor_indices[neighbor_num] = (current_index - GRID_ROWS);
        neighbor_num++;
    }
    if(!right_edge) {
        neighbor_indices[neighbor_num] = (current_index + GRID_ROWS);
        neighbor_num++;
    }

    // PICK RANDOM DIRECTION, FROM AVAILABLE
    int random_index = rand() % (neighbor_num);
    return neighbor_indices[random_index];
}

void DrawBorder(SDL_Renderer *renderer, cell* cells, int x, int y) {
    const int grid_color = 50;
    const int cell_size = cells[(x * GRID_COLUMNS) + y].size;
    const int borders = cells[(x * GRID_COLUMNS) + y].borders;
    const int start_x = cells[(x * GRID_COLUMNS) + y].start_x;
    const int start_y = cells[(x * GRID_COLUMNS) + y].start_y;

    // Top Edge
    const int top_bit = borders & (1 << Up);
    const int top_color = top_bit == 0 ? grid_color : 0;
    const SDL_FRect top_edge = { 
        .x = top_bit == 0 ? start_x + GRID_CELL_BORDER_SIZE : start_x, 
        .y = start_y, 
        .w = cell_size,
        .h = GRID_CELL_BORDER_SIZE 
    };
    SDL_SetRenderDrawColor(renderer, top_color, top_color, top_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &top_edge);

    // Bottom Edge
    const int bottom_bit = borders & (1 << Down);
    const int bottom_color = bottom_bit == 0 ? grid_color : 0;
    const SDL_FRect bottom_edge = { 
        .x = bottom_bit == 0 ? start_x + GRID_CELL_BORDER_SIZE : start_x,
        .y = start_y + cell_size,
        .w = bottom_bit == 0 ? cell_size : cell_size + GRID_CELL_BORDER_SIZE,
        .h = GRID_CELL_BORDER_SIZE 
    };
    SDL_SetRenderDrawColor(renderer, bottom_color, bottom_color, bottom_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &bottom_edge);

    // Left Edge
    const int left_bit = borders & (1 << Left);
    const int left_color = left_bit == 0 ? grid_color : 0;
    const SDL_FRect left_edge = { 
        .x = start_x,
        .y = left_bit == 0 ? start_y + GRID_CELL_BORDER_SIZE : start_y, 
        .w = GRID_CELL_BORDER_SIZE,
        .h = left_bit == 0 ? cell_size - GRID_CELL_BORDER_SIZE : cell_size,
    };
    SDL_SetRenderDrawColor(renderer, left_color, left_color, left_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &left_edge);

    // Right Edge
    const int right_bit = borders & (1 << Right);
    const int right_color = right_bit == 0 ? grid_color : 0;
    const SDL_FRect right_edge = { 
        .x = start_x + cell_size, 
        .y = right_bit == 0 ? start_y + GRID_CELL_BORDER_SIZE : start_y, 
        .w = GRID_CELL_BORDER_SIZE, 
        .h = right_bit == 0 ? cell_size - GRID_CELL_BORDER_SIZE : cell_size,
    };
    SDL_SetRenderDrawColor(renderer, right_color, right_color, right_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &right_edge);
}

void DrawSquare(SDL_Renderer *renderer, cell* cells, int x, int y) {
    const int square_size = cells[(x * GRID_COLUMNS) + y].size - GRID_CELL_BORDER_SIZE;
    SDL_FRect inner_square = { 
        .x = cells[(x * GRID_COLUMNS) + y].start_x + GRID_CELL_BORDER_SIZE, 
        .y = cells[(x * GRID_COLUMNS) + y].start_y + GRID_CELL_BORDER_SIZE, 
        .w = square_size, .h = square_size
    };
    SDL_RenderFillRect(renderer, &inner_square);
}