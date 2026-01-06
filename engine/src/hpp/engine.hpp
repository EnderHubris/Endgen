#ifndef ENDGEN_ENGINE
#define ENDGEN_ENGINE

#include <SDL.h>
#include <iostream>
#include <scene.hpp>

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

        int WIDTH;
        int HEIGHT;

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        
        EndgenScene* scene;
};

#endif