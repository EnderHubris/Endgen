#ifndef ENDGEN_ENGINE
#define ENDGEN_ENGINE

#include <SDL.h>
#include <SDL_stdinc.h>

#include <iostream>
#include <scene.hpp>

class EndgenEngine
{
    public:
        EndgenEngine(bool inspectorMode = false, int w = 800, int h = 600);
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

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        
        EndgenScene* scene;
};

#endif