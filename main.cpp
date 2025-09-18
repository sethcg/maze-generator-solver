#define SDL_MAIN_USE_CALLBACKS 1 // USE CALLBACKS INSTEAD OF THE "main()" FUNCTION

#include <stdlib.h>
#include <time.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

extern "C" {
    #include <Grid.h>
    #include <WilsonMaze.h>
}

#define WINDOW_WIDTH 840
#define WINDOW_HEIGHT 840

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
cell* cells = NULL;

// THIS FUNCTION RUNS ONCE AT STARTUP
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    srand(time(NULL));

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Maze Generation", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if(cells == NULL) {
        cells = (cell*) malloc(GRID_ARRAY_SIZE * sizeof(cell));
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.LogFilename = NULL;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // CREATE/DRAW GRID
    CreateGrid(renderer, cells);
    
    /*********************************************************************/
    SDL_Log("Generating New Maze...");    
    int* remaining = (int*) malloc(sizeof(int));
    Allocate_WilsonMaze(remaining, cells);
    // ITERATE THROUGH WILSON MAZE
    bool iterate = true;
    while(iterate) {
        iterate = Iterate_WilsonMaze(remaining, renderer, cells);
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE_FLOAT);
        SDL_RenderClear(renderer);
        DrawGrid(renderer, cells);
        SDL_RenderPresent(renderer);
    };
    Free_WilsonMaze();
    free(remaining);
    
    // SET BACKGROUND COLOR
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    // DRAW CURRENT GRID
    DrawGrid(renderer, cells);

    SDL_RenderPresent(renderer);
    /*********************************************************************/

    return SDL_APP_CONTINUE;
}

// THIS FUNCTION RUNS WHEN A NEW EVENT OCCURS (MOUSE INPUT, KEY PRESS, ETC.)
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event* event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
    }

    ImGui_ImplSDL3_ProcessEvent(event);

    return SDL_APP_CONTINUE;
}

// THIS FUNCTION RUNS ONCE PER FRAME
SDL_AppResult SDL_AppIterate(void *appstate) {
    // ImGui_ImplSDLRenderer3_NewFrame();
    // ImGui_ImplSDL3_NewFrame();
    // ImGui::NewFrame();

    // ImGui::Begin("Another Window");
    // if (ImGui::Button("Generate Maze")) {
    //     SDL_Log("Generating Maze...");
    // }
    // ImGui::End();

    // // RENDERING
    // ImGui::Render();
    // SDL_RenderClear(renderer);
    // SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE_FLOAT);
    // ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    // SDL_RenderPresent(renderer);
    
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(cells);
}