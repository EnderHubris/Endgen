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
    if (renderer == nullptr || font == nullptr) {
        std::cerr << "[-] Could not create Text!" << std::endl;
        return nullptr;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) return nullptr;

    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surface);
    outRect.w = surface->w;
    outRect.h = surface->h;

    SDL_FreeSurface(surface);
    return message;
}

//======================================================================

Inspector::Inspector(): renderer(nullptr), texture(nullptr), font(nullptr), closed(false) {}

void Inspector::StartUp() {
    std::cout << "[*] Starting Inspector. . ." << std::endl;
    Run();
}

void Inspector::SetRenderer(SDL_Renderer* r) { renderer = r; };
void Inspector::SetTexture(SDL_Texture* t) { texture = t; };
void Inspector::SetFont(TTF_Font* f) { font = f; };
void Inspector::SetWindow(SDL_Window* w) { window = w; };

void Inspector::Run() {
    std::cout << "[*] Rendering Inspector Panel. . ." << std::endl;
    running = true;
    
    if (window == nullptr || renderer == nullptr || texture == nullptr || font == nullptr) {
        std::cout << "[-] Inspector Components not Defined!" << std::endl;
        return;
    }

    //========== UI COMPONENTS ===========
    button.rect = { 300, 260, 200, 60 };
    button.text = "Click Me";

    SDL_Rect textRect;
    SDL_Color white{255,255,255,255};

    SDL_Texture* textTexture = Inspector_UI::CreateText(renderer, font, button.text, white, textRect);
    if (!textTexture) {
        std::cout << "[-] Failed to create Text Texture!" << std::endl;
        return;
    }
    SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);

    textRect.x = button.rect.x + (button.rect.w - textRect.w) / 2;
    textRect.y = button.rect.y + (button.rect.h - textRect.h) / 2;
    //====================================

    SDL_Event event;
    bool focused = false;
    while (running && engineRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT) {
                Uint32 id = event.window.windowID;
                bool closeWindow = event.window.event == SDL_WINDOWEVENT_CLOSE;

                if (id == SDL_GetWindowID(window)) {
                    if (focused && closeWindow) {
                        running = false;
                    }

                    if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                        focused = true;
                    } else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                        focused = false;
                    }
                }
            }

            if (focused && event.type == SDL_MOUSEMOTION) {
                button.hovered = Inspector_UI::PointInRect(
                    event.motion.x,
                    event.motion.y,
                    button.rect
                );
            }

            if (focused && event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT) {
                if (Inspector_UI::PointInRect(event.button.x, event.button.y, button.rect)) {
                    std::cout << "Button clicked!" << std::endl;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Button background
        SDL_SetRenderDrawColor(renderer, button.hovered ? 100 : 70, 70, 70, 255);
        SDL_RenderFillRect(renderer, &button.rect);

        // Button border
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderDrawRect(renderer, &button.rect);

        // Button text
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
    
    SDL_DestroyTexture(textTexture);

    std::cout << "[*] Inspector Rendering Closing. . ." << std::endl;
    CloseInspector();
}

void Inspector::CloseInspector() {
    if (!closed) {
        closed = true;
        std::cout << "[*] Closing Inspector. . ." << std::endl;
        
        if (window != nullptr) { SDL_DestroyWindow(window); }
        std::cout << " ├─ Closed Window Successfully!" << std::endl;
        
        if (texture != nullptr) { SDL_DestroyTexture(texture); }
        std::cout << " ├─ Closed Texture Successfully!" << std::endl;
        
        if (renderer != nullptr) { SDL_DestroyRenderer(renderer); }
        std::cout << " ├─ Closed Renderer Successfully!" << std::endl;
        
        if (font != nullptr) { TTF_CloseFont(font); }
        std::cout << " ├─ Closed Font Successfully!" << std::endl;

        std::cout << "[+] Inspector Closed Successfully!\n\n";
    }
}

Inspector::~Inspector() {
    CloseInspector();
}