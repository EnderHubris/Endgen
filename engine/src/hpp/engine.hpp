#ifndef ENDGEN_ENGINE
#define ENDGEN_ENGINE

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_stdinc.h>

#include <iostream>
#include <scene.hpp>

namespace SDL_Helper {
    SDL_Window* Create_Window(const char* WINDOW_NAME, int x, int y, int width, int height);
    SDL_Renderer* Create_Renderer(SDL_Window* window, bool useGPU);
    SDL_Texture* Create_Texture(SDL_Renderer* renderer, int width, int height);
    TTF_Font* Create_Font(
        const char* font_path = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        int fontSize = 24
    );
};

class EndgenEngine
{
    public:
        EndgenEngine(int w = 800, int h = 600);
        ~EndgenEngine();

    private:
        // initialize engine components to then be shared
        bool Init();
        // runs the engine loop
        void Run();
        // renders a given scene
        void RenderScene();
        // listen for player input
        void GetInput();

        int WIDTH;
        int HEIGHT;

        SDL_Window*     window;
        SDL_Renderer*   renderer;
        SDL_Texture*    texture;
        TTF_Font*       font;
        
        EndgenScene*    scene;
};

#endif