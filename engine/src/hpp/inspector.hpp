#ifndef ENDGEN_INSPECTOR
#define ENDGEN_INSPECTOR

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_stdinc.h>

#include <iostream>
#include <string>

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

class Inspector {
    public:
        Inspector(int w = 800, int h = 600);
        ~Inspector();
    private:
        bool Init();
        void Run();

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        TTF_Font* font;

        int WIDTH;
        int HEIGHT;
};

#endif