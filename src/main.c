#define SDL_MAIN_USE_CALLBACKS 1 // USE CALLBACKS INSTEAD OF THE "main()" FUNCTION

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// WINDOW CONSTANTS
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// GRID CONSTANTS
const int GRID_SQUARE_SIZE = 32;
const int GRID_PADDING_SIZE = 16;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

void DrawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE_FLOAT);

    // ADD ROWS
    SDL_FRect row_line = { .x = GRID_PADDING_SIZE, .y = 0, .w = (WINDOW_WIDTH - (GRID_PADDING_SIZE * 2)), .h = 0 };
    for(row_line.y = GRID_PADDING_SIZE; row_line.y <= (WINDOW_HEIGHT - GRID_PADDING_SIZE); row_line.y += GRID_SQUARE_SIZE) {
        SDL_RenderRect(renderer, &row_line);
    }

    // ADD COLUMNS
    SDL_FRect col_line = { .x = 0, .y = GRID_PADDING_SIZE, .w = 0, .h = (WINDOW_HEIGHT - (GRID_PADDING_SIZE * 2)) };
    for(col_line.x = GRID_PADDING_SIZE; col_line.x <= (WINDOW_WIDTH - GRID_PADDING_SIZE); col_line.x += GRID_SQUARE_SIZE) {
        SDL_RenderRect(renderer, &col_line);
    }
}

// THIS FUNCTION RUNS ONCE AT STARTUP
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("SDL3 Grid", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // SET BACKGROUND COLOR (DARK GRAY)
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    // DRAW GRID
    DrawGrid(renderer);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

// THIS FUNCTION RUNS WHEN A NEW EVENT OCCURS (MOUSE INPUT, KEY PRESS, ETC.)
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event -> type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

// THIS FUNCTION RUNS ONCE PER FRAME
SDL_AppResult SDL_AppIterate(void *appstate)
{
    // // SET BACKGROUND COLOR (DARK GRAY)
    // SDL_SetRenderDrawColorFloat(renderer, 80.0f, 80.0f, 80.0f, SDL_ALPHA_OPAQUE_FLOAT);
    // SDL_RenderClear(renderer);

    // // DrawGrid(renderer, 16);

    // SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

// THIS FUNCTION RUNS ONCE AT SHUTDOWN
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // SDL CLEANS UP WINDOW/RENDERER AUTO-MAGICALLY
}