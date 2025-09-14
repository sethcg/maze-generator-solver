#ifndef CELL_H
#define CELL_H

typedef enum {
    Up,
    Down,
    Left,
    Right,
    None
} direction;

typedef struct {
    int index;
    int start_x;    // TOP-LEFT X COORDINATE
    int start_y;    // TOP-LEFT Y COORDINATE
    int size;
    int borders;    // INT/BIT VALUE REPRESENTING NORTH, SOUTH, EAST, WEST
    bool visited;
    direction dir;
} cell;

#define create_cell() { .visited = false, .dir = None , .borders = 0b1111 };

#endif