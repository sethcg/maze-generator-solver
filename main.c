#define SDL_MAIN_USE_CALLBACKS 1 // USE CALLBACKS INSTEAD OF THE "main()" FUNCTION

#include <stdlib.h>
#include <time.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <grid.h>
#include <wilson_maze.h>

#define WINDOW_WIDTH 840
#define WINDOW_HEIGHT 840

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

cell* cells = NULL;

// THIS FUNCTION RUNS ONCE AT STARTUP
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    srand(time(NULL));

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("SDL3 Grid", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if(cells == NULL) {
        cells = malloc((GRID_ROWS * GRID_COLUMNS) * sizeof(cell));
    }

    // CREATE/DRAW MAZE
    CreateGrid(renderer, cells);
    DrawMaze(renderer, cells);
    
    return SDL_APP_CONTINUE;
}

// THIS FUNCTION RUNS WHEN A NEW EVENT OCCURS (MOUSE INPUT, KEY PRESS, ETC.)
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

// THIS FUNCTION RUNS ONCE PER FRAME
SDL_AppResult SDL_AppIterate(void *appstate)
{
    return SDL_APP_CONTINUE;
}

// THIS FUNCTION RUNS ONCE AT SHUTDOWN
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // SDL CLEANS UP WINDOW/RENDERER AUTO-MAGICALLY

    free(cells);
}