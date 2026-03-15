#ifndef ENDGEN_SCENE
#define ENDGEN_SCENE

#include <SDL.h>
#include <SDL_stdinc.h> // import types: Uint32, Uint8, etc

#include <vector>
#include <cstring>
#include <algorithm>

// engine types
#include <vec.hpp>

// engine components
#include <camera.hpp>
#include <basic_objects.hpp>

class EndgenScene {
    public:
        EndgenScene(int w, int h, SDL_Renderer* rend, SDL_Texture* text);
        ~EndgenScene();

        void Render();

        // set starting position of Scene Camera
        // before Rendering Scene
        void SetCamera(Vector3 camPos);

        SDL_Renderer* GetRenderer();
        SDL_Texture* GetTexture();

        std::vector<Uint32>& GetBuffer();

    private:
        Camera* MainCamera;

        int WIDTH;
        int HEIGHT;
        
        SDL_Renderer* renderer;
        SDL_Texture* texture;

        std::vector<Triangle> sceneObjects;
        
        // buffer is used for drawing to the texture
        std::vector<Uint32> pixelBuffer;
};

#endif