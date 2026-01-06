#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    // Initialize SDL (video subsystem)
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create the window
    SDL_Window* window = SDL_CreateWindow(
        "Game Window",              // Window Name
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,                        // Window Width
        600,                        // Window Height
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        std::cerr << "Error creating SDL window failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Event loop
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_Delay(16); // ~60 FPS, prevents 100% CPU usage
    }

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}