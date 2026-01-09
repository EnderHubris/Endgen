#ifndef ENDGEN_INSPECTOR
#define ENDGEN_INSPECTOR

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_stdinc.h>

#include <iostream>
#include <string>

#include <functional>
#include <atomic>

#include <scene.hpp>

inline std::atomic_bool engineRunning{true};

namespace Inspector_UI {
    struct Button {
        SDL_Rect rect;
        SDL_Rect textRect;
        std::string text;
        bool hovered = false;
        SDL_Texture* textTexture = nullptr;
        std::function<void()> onClick = [](){ std::cout << "Button clicked!" << std::endl; };
    };

    bool PointInRect(int x, int y, const SDL_Rect& r);

    SDL_Texture* CreateText(
        SDL_Renderer* renderer,
        TTF_Font* font,
        std::string text,
        SDL_Color color,
        SDL_Rect& outRect
    );

    Button* CreateBtn(
        SDL_Rect rect,
        std::string text,
        SDL_Color textColor,
        SDL_Renderer* renderer,
        TTF_Font* font
    );

    struct TextField {
        SDL_Rect rect;
        SDL_Rect textRect;
        std::string text;
        SDL_Texture* textTexture = nullptr;
    };

    TextField* CreateTextField(
        SDL_Rect rect,
        std::string text,
        SDL_Color textColor,
        SDL_Renderer* renderer,
        TTF_Font* font
    );

    void UpdateTextField(
        Inspector_UI::TextField* field,
        SDL_Renderer* renderer,
        TTF_Font* font,
        SDL_Color color
    );
};

class Inspector {
    public:
        Inspector();
        ~Inspector();

        void SetRenderer(SDL_Renderer* r);
        void SetWindow(SDL_Window* w);
        void SetTexture(SDL_Texture* t);
        void SetFont(TTF_Font* f);
        void SetActiveScene(EndgenScene* scene);

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

        EndgenScene* activeScene;

        // UI Elements
        std::vector<Inspector_UI::Button*> buttons;
        std::vector<Inspector_UI::TextField*> textDisplays;
};

#endif