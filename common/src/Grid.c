#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL3/SDL.h>
#include <Grid.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

void SetBackground(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);
}

void CreateGrid(SDL_Renderer *renderer, cell* cells, int window_width, int window_height) {
    const int cell_total_size = GRID_CELL_SIZE + (GRID_CELL_BORDER_SIZE * 2);
    for (int x = 0; x < GRID_ROWS; x++) {
        for (int y = 0; y < GRID_COLUMNS; y++) {
            cells[(x * GRID_COLUMNS) + y].index = (x * GRID_COLUMNS) + y;
            cells[(x * GRID_COLUMNS) + y].start_x = GRID_PADDING_SIZE + (x * cell_total_size);
            cells[(x * GRID_COLUMNS) + y].start_y = GRID_PADDING_SIZE + (y * cell_total_size);
            cells[(x * GRID_COLUMNS) + y].size = cell_total_size;
            cells[(x * GRID_COLUMNS) + y].visited = false;

            // SET ALL BORDER BITS TO "TRUE"
            cells[(x * GRID_COLUMNS) + y].borders = 0b1111;

            // ADD BORDERS
            DrawBorder(renderer, cells, x, y);

            // ADD SQUARE
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE_FLOAT);
            DrawSquare(renderer, cells, x, y);
        }
    }
}

void DrawGrid(SDL_Renderer *renderer, cell* cells) {
    SetBackground(renderer);

    for (int x = 0; x < GRID_ROWS; x++) {
        for (int y = 0; y < GRID_COLUMNS; y++) {

            // ADD BORDERS
            DrawBorder(renderer, cells, x, y);

            // ADD SQUARE
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE_FLOAT);
            DrawSquare(renderer, cells, x, y);
        }
    }

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
    srand(time(NULL));
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
    srand(time(NULL));
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

        bool makes_loop = contains(next_index, array_size_max, indices);
        if(makes_loop) {
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
                next_index == (current_index + 1) ? South :
                next_index == (current_index - 1) ? North :
                next_index == (current_index - GRID_ROWS) ? West :
                next_index == (current_index + GRID_ROWS) ? East : None;
            running_index++;
        }

        current_index = next_index;
    }

    // APPLY THE PATH
    for(int i = 0; i < running_index; i++) {
        direction dir = directions[i];

        int current = indices[i];
        int next = 
            dir == North ? (current - 1) :
            dir == South ? (current + 1) :
            dir == East ? (current + GRID_ROWS) :
            dir == West ? (current - GRID_ROWS) : -1;

        // MARK CURRENT AS VISITED
        cells[current].visited = true;

        // APPLY BORDER CHANGES, TO CURRENT AND NEXT
        cells[current].borders = 
            dir == North ? cells[current].borders ^ (1 << 0) :
            dir == South ? cells[current].borders ^ (1 << 1) :
            dir == West ? cells[current].borders ^ (1 << 2) :
            dir == East ? cells[current].borders ^ (1 << 3) : cells[current].borders;
        
        // APPLY OPPOSITE TO THE NEXT BORDER
        cells[next].borders = 
            dir == South ? cells[next].borders ^ (1 << 0) :
            dir == North ? cells[next].borders ^ (1 << 1) :
            dir == East ? cells[next].borders ^ (1 << 2) :
            dir == West ? cells[next].borders ^ (1 << 3) : cells[next].borders;

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
    srand(time(NULL));
    int random_index = rand() % (neighbor_num);
    return neighbor_indices[random_index];
}

void DrawBorder(SDL_Renderer *renderer, cell* cells, int x, int y) {
    const int grid_color = 50;
    const int cell_size = cells[(x * GRID_COLUMNS) + y].size;
    const int borders = cells[(x * GRID_COLUMNS) + y].borders;
    const int start_x = cells[(x * GRID_COLUMNS) + y].start_x;
    const int start_y = cells[(x * GRID_COLUMNS) + y].start_y;

    // NORTH
    const int n_bit = borders & (1 << 0);
    const int n_color = n_bit == 0 ? grid_color : 0;
    const SDL_FRect north = { 
        .x = n_bit == 0 ? start_x + GRID_CELL_BORDER_SIZE : start_x, 
        .y = start_y, 
        .w = cell_size,
        .h = GRID_CELL_BORDER_SIZE 
    };
    SDL_SetRenderDrawColor(renderer, n_color, n_color, n_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &north);

    // SOUTH
    const int s_bit = borders & (1 << 1);
    const int s_color = s_bit == 0 ? grid_color : 0;
    const SDL_FRect south = { 
        .x = s_bit == 0 ? start_x + GRID_CELL_BORDER_SIZE : start_x,
        .y = start_y + cell_size,
        .w = cell_size,
        .h = GRID_CELL_BORDER_SIZE 
    };
    SDL_SetRenderDrawColor(renderer, s_color, s_color, s_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &south);

    // EAST
    const int e_bit = borders & (1 << 2);
    const int e_color = e_bit == 0 ? grid_color : 0;
    const SDL_FRect east = { 
        .x = start_x,
        .y = e_bit == 0 ? start_y + GRID_CELL_BORDER_SIZE : start_y, 
        .w = GRID_CELL_BORDER_SIZE,
        .h = cell_size,
    };
    SDL_SetRenderDrawColor(renderer, e_color, e_color, e_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &east);

    // WEST
    const int w_bit = borders & (1 << 3);
    const int w_color = w_bit == 0 ? grid_color : 0;
    const SDL_FRect west = { 
        .x = start_x + cell_size, 
        .y = w_bit == 0 ? start_y + GRID_CELL_BORDER_SIZE : start_y, 
        .w = GRID_CELL_BORDER_SIZE, 
        .h = cell_size,
    };
    SDL_SetRenderDrawColor(renderer, w_color, w_color, w_color, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderFillRect(renderer, &west);
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