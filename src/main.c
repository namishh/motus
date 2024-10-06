#include <SDL2/SDL.h>

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Minimal SDL2 Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
