#ifndef ENDGEN_INSPECTOR
#define ENDGEN_INSPECTOR

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_stdinc.h>

#include <iostream>
#include <string>

#include <mutex>
#include <atomic>

namespace Inspector_UI {
    struct Button {
        SDL_Rect rect;
        std::string text;
        bool hovered = false;
    };

    bool PointInRect(int x, int y, const SDL_Rect& r);

    SDL_Texture* CreateText(
        SDL_Renderer* renderer,
        TTF_Font* font,
        const std::string& text,
        SDL_Color color,
        SDL_Rect& outRect
    );
};

inline std::atomic_bool engineRunning{true};
inline std::mutex sceneObjectMutex;

class Inspector {
    public:
        Inspector();
        ~Inspector();

        void SetRenderer(SDL_Renderer* r);
        void SetWindow(SDL_Window* w);
        void SetTexture(SDL_Texture* t);
        void SetFont(TTF_Font* f);

        void StartUp();
        void CloseInspector();
        
    private:
        void Run();

        bool running;
        bool closed;

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        TTF_Font* font;

        // UI Elements
        Inspector_UI::Button button;
};

#endif