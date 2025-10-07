#ifndef CELL_H
#define CELL_H

typedef enum {
    Up,
    Down,
    Right,
    Left,
    None
} direction;

typedef struct {
    int index;
    int start_x;        // TOP-LEFT X COORDINATE
    int start_y;        // TOP-LEFT Y COORDINATE
    int size;
    uint8_t borders;    // FIRST 4-BITS: [UP, DOWN, RIGHT, LEFT]
    bool visited;
} cell;

typedef struct {
    int cell_index;
    direction dir;
} cell_direction;

#define create_cell() { .visited = false, .borders = (uint8_t) 0b1111 };

#endif