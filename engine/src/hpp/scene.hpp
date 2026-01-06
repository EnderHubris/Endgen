#ifndef ENDGEN_SCENE
#define ENDGEN_SCENE

#include <SDL.h>
#include <SDL_stdinc.h>

// engine types
#include <vec3.hpp>

// engine components
#include <camera.hpp>

class EndgenScene
{
    public:
        EndgenScene(int w, int h, SDL_Renderer* rend, SDL_Texture* text);
        ~EndgenScene();

        void Render();

    private:
        Camera* MainCamera;

        int WIDTH;
        int HEIGHT;
        
        SDL_Renderer* renderer;
        SDL_Texture* texture;
};

#endif