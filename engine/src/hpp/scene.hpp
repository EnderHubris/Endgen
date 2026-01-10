#ifndef ENDGEN_SCENE
#define ENDGEN_SCENE

#include <SDL.h>
#include <SDL_stdinc.h> // import types: Uint32, Uint8, etc

#include <vector>
#include <cstring>
#include <thread>
#include <atomic>
#include <mutex>

// engine types
#include <vec3.hpp>

// engine components
#include <camera.hpp>
#include <basic_objects.hpp>

inline std::mutex sceneObjectMutex;
inline std::atomic_bool runRenderer{true};

inline std::mutex pixelBufferMutex;
inline std::atomic_bool bufferReady{false};

namespace RenderMath {
    bool RayIntersectsAABB(
        const Vector3& origin,
        const Vector3& dir,
        const Vector3& min,
        const Vector3& max,
        float& tOut
    );
};

class EndgenScene
{
    public:
        EndgenScene(int w, int h, SDL_Renderer* rend, SDL_Texture* text);
        ~EndgenScene();

        void Render();

        // set starting position of Scene Camera
        // before Rendering Scene
        void SetCamera(Vector3 camPos);
        void StopRenderer();
        std::vector<WorldObject*>* GetSceneObjects();
        size_t ObjectCount() const;
    
        Vector3 RayFromCamera(int x, int y);
        Uint32 RaycastScene(const Vector3& rayDir);

        SDL_Renderer* GetRenderer();
        SDL_Texture* GetTexture();

        std::vector<Uint32>& GetBuffer();

    private:
        Camera* MainCamera;

        int WIDTH;
        int HEIGHT;
        
        SDL_Renderer* renderer;
        SDL_Texture* texture;

        std::vector<WorldObject*> sceneObjects;
        
        std::vector<Uint32> pixelBuffer;    // buffer is used for drawing to the texture
        std::vector<Uint32> internalBuffer; // buffer is the destination of render computations

        std::thread rThread;
};

#endif