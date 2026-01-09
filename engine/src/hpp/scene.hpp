#ifndef ENDGEN_SCENE
#define ENDGEN_SCENE

#include <SDL.h>
#include <SDL_stdinc.h> // import types: Uint32, Uint8, etc

#include <vector>
#include <mutex>

// engine types
#include <vec3.hpp>

// engine components
#include <camera.hpp>
#include <basic_objects.hpp>

inline std::mutex sceneObjectMutex;

class EndgenScene
{
    public:
        EndgenScene(int w, int h, SDL_Renderer* rend, SDL_Texture* text);
        ~EndgenScene();

        void Render();

        // set starting position of Scene Camera
        // before Rendering Scene
        void SetCamera(Vector3 camPos);
        std::vector<WorldObject*>* GetSceneObjects();
        size_t ObjectCount() const;
    
        Vector3 RayFromCamera(int x, int y);
        Uint32 RaycastScene(const Vector3& rayDir);

    private:
        Camera* MainCamera;

        int WIDTH;
        int HEIGHT;
        
        SDL_Renderer* renderer;
        SDL_Texture* texture;

        std::vector<WorldObject*> sceneObjects;
};

#endif