#include <stdlib.h>
#include <math.h>

int* GenerateMaze(int arraySize) {
    // const int above = 1 << 0;
    // const int left = 1 << 1;
    // const int below = 1 << 2;
    // const int right = 1 << 3;

    int* cells = (int*)calloc(arraySize, sizeof(int));
    int* remainingCells = (int*)malloc(arraySize * sizeof(int));
    int* previous = (int*)malloc(arraySize * sizeof(int));

    int remaining = arraySize;
    // while(remaining != 0) {
    //     remaining = randomWalk();
    // }

    free(remainingCells);
    free(previous);

    return cells;
}

