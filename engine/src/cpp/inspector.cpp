#include <inspector.hpp>

bool Inspector_UI::PointInRect(int x, int y, const SDL_Rect& r) {
    return x >= r.x && x <= r.x + r.w &&
        y >= r.y && y <= r.y + r.h;
}

SDL_Texture* Inspector_UI::CreateText(
    SDL_Renderer* renderer,
    TTF_Font* font,
    const std::string& text,
    SDL_Color color,
    SDL_Rect& outRect
) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) return nullptr;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    outRect.w = surface->w;
    outRect.h = surface->h;

    SDL_FreeSurface(surface);
    return texture;
}

Inspector::Inspector(int w, int h): WIDTH(w), HEIGHT(h) {
    std::cout << "[*] Starting Inspector. . ." << std::endl;
    if (!Init()) return;

    Run();
}

bool Inspector::Init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "[-] SDL failed to initialize: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() != 0) {
        std::cerr << "[-] TTF_Init failed" << std::endl;
        return false;
    }

    // Create window
    window = SDL_CreateWindow(
        "Inspector Window",
        SDL_WINDOWPOS_CENTERED + 50,
        SDL_WINDOWPOS_CENTERED + 50,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "[-] Inspector Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        std::cerr << "[-] Inspector Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    if (!texture) {
        std::cerr << "[-] Texture creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    font = TTF_OpenFont(
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24
    );

    if (!font) {
        std::cerr << "[-] Failed to load Font" << std::endl;
        return false;
    }

    return true;
}

void Inspector::Run() {
    bool running = true;
    SDL_Event event;
    std::cout << "[*] Rendering Inspector Panel. . ." << std::endl;

    //========== UI COMPONENTS ===========
    Inspector_UI::Button button;
    button.rect = { 300, 260, 200, 60 };
    button.text = "Click Me";

    SDL_Rect textRect;
    SDL_Color white{255,255,255,255};

    SDL_Texture* textTexture = Inspector_UI::CreateText(renderer, font, button.text, white, textRect);

    textRect.x = button.rect.x + (button.rect.w - textRect.w) / 2;
    textRect.y = button.rect.y + (button.rect.h - textRect.h) / 2;
    //====================================

    while (running) {
        // listen for events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_MOUSEMOTION) {
                button.hovered = Inspector_UI::PointInRect(
                    event.motion.x,
                    event.motion.y,
                    button.rect
                );
            }

            if (event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT) {
                if (Inspector_UI::PointInRect(event.button.x, event.button.y, button.rect)) {
                    std::cout << "Button clicked!" << std::endl;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Button background
        if (button.hovered)
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        else
            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);

        SDL_RenderFillRect(renderer, &button.rect);

        // Button border
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderDrawRect(renderer, &button.rect);

        // Button text
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
}

Inspector::~Inspector() {
    std::cout << "[*] Closing Inspector. . ." << std::endl;

    if (window != nullptr) {
        SDL_DestroyWindow(window);
    }
    std::cout << " ├─ Closed Window Successfully!" << std::endl;

    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
    std::cout << " ├─ Closed Texture Successfully!" << std::endl;

    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }
    std::cout << " ├─ Closed Renderer Successfully!" << std::endl;

    if (font != nullptr) {
        TTF_CloseFont(font);
    }
    std::cout << " ├─ Closed Font Successfully!" << std::endl;

    TTF_Quit();
    SDL_Quit();

    std::cout << "[+] Inspector Closed Successfully!" << std::endl;
}