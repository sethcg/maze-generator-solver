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
    int r;
    int g;
    int b;
} rgb_color;

#define create_rgb_color(r_value, g_value, b_value) { .r = r_value, .g = g_value, .b = b_value };

typedef struct {
    int cell_index;
    direction dir;
} cell_direction;

typedef struct {
    int index;
    int start_x;        // TOP-LEFT X COORDINATE
    int start_y;        // TOP-LEFT Y COORDINATE
    int size;
    uint8_t borders;    // FIRST 4-BITS: [UP, DOWN, RIGHT, LEFT]
    bool visited;

    bool path_cell;
    rgb_color path_gradient;
} cell;

#define create_cell() { .visited = false, .borders = (uint8_t) 0b1111, .path_cell = false };

#endif