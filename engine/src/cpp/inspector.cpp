#include <inspector.hpp>

bool Inspector_UI::PointInRect(int x, int y, const SDL_Rect& r) {
    return x >= r.x && x <= r.x + r.w &&
        y >= r.y && y <= r.y + r.h;
}

SDL_Texture* Inspector_UI::CreateText(
    SDL_Renderer* renderer,
    TTF_Font* font,
    std::string text,
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

Inspector_UI::Button* Inspector_UI::CreateBtn(
    SDL_Rect rect, // // ex. { 300, 260, 200, 60 }
    std::string text,
    SDL_Color textColor, // ex. {255,255,255,255}
    SDL_Renderer* renderer,
    TTF_Font* font
) {
    Inspector_UI::Button* button = new Inspector_UI::Button();
    button->rect = rect;
    button->text = text;

    button->textTexture = Inspector_UI::CreateText(
        renderer,
        font,
        button->text,
        textColor,
        button->textRect
    );

    if (!button->textTexture) {
        std::cout << "[-] Failed to create Text Texture!" << std::endl;
    }
    SDL_SetTextureBlendMode(button->textTexture, SDL_BLENDMODE_BLEND);

    button->textRect.x = button->rect.x + (button->rect.w - button->textRect.w) / 2;
    button->textRect.y = button->rect.y + (button->rect.h - button->textRect.h) / 2;
    return button;
}

Inspector_UI::TextField* Inspector_UI::CreateTextField(
    SDL_Rect rect,
    std::string text,
    SDL_Color textColor,
    SDL_Renderer* renderer,
    TTF_Font* font
) {
    Inspector_UI::TextField* textField = new Inspector_UI::TextField();
    textField->rect = rect;
    textField->text = text;

    textField->textTexture = Inspector_UI::CreateText(
        renderer,
        font,
        textField->text,
        textColor,
        textField->textRect
    );

    if (!textField->textTexture) {
        std::cout << "[-] Failed to create Text Texture!" << std::endl;
    }
    SDL_SetTextureBlendMode(textField->textTexture, SDL_BLENDMODE_BLEND);

    textField->textRect.x = textField->rect.x + (textField->rect.w - textField->textRect.w) / 2;
    textField->textRect.y = textField->rect.y + (textField->rect.h - textField->textRect.h) / 2;
    return textField;
}

void Inspector_UI::UpdateTextField(
        Inspector_UI::TextField* field,
        SDL_Renderer* renderer,
        TTF_Font* font,
        SDL_Color color
    ) {
        if (!field) return;

        if (field->textTexture) {
            SDL_DestroyTexture(field->textTexture);
            field->textTexture = nullptr;
        }

        field->textTexture = Inspector_UI::CreateText(
            renderer,
            font,
            field->text,
            color,
            field->textRect
        );

        // re-center text
        field->textRect.x =
            field->rect.x + (field->rect.w - field->textRect.w) / 2;
        field->textRect.y =
            field->rect.y + (field->rect.h - field->textRect.h) / 2;
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
void Inspector::SetActiveScene(EndgenScene* scene) {
    activeScene = scene;
};

void Inspector::Run() {
    std::cout << "[*] Rendering Inspector Panel. . ." << std::endl;
    running = true;
    
    if (window == nullptr || renderer == nullptr || texture == nullptr || font == nullptr) {
        std::cout << "[-] Inspector Components not Defined!" << std::endl;
        return;
    }

    int c = (activeScene != nullptr) ? activeScene->ObjectCount() : -1;

    auto CreatePositionPanel = [](
        SDL_Renderer* renderer,
        TTF_Font* font,
        std::vector<Inspector_UI::Button*>& btns,
        std::vector<Inspector_UI::TextField*>& txts,
        std::string text,
        int x,
        int y,
        std::function<void()> incEvt = [](){},
        std::function<void()> decEvt = [](){}
    ) {
        txts.push_back(
            Inspector_UI::CreateTextField(
                { x, y, 200, 60 },
                text,
                {255,255,255,255},
                renderer,
                font
            )
        );

        btns.push_back(
            Inspector_UI::CreateBtn(
                { x, y - 75, 200, 60 },
                "Increase",
                {255,255,255,255},
                renderer,
                font
            )
        );
        btns.back()->onClick = incEvt;

        btns.push_back(
            Inspector_UI::CreateBtn(
                { x, y + 75, 200, 60 },
                "Decrease",
                {255,255,255,255},
                renderer,
                font
            )
        );
        btns.back()->onClick = decEvt;
    };

    //========== UI COMPONENTS ===========
    for (size_t i = 0; i < c; ++i) {
        WorldObject* obj = activeScene->GetSceneObjects()->at(i);
        if (obj == nullptr) continue;

        int textPositionX = 65;

        int yOffset = 250 * (int)i;
        int textPositionY = 100 + yOffset;

        // positions (x,y,z)
        CreatePositionPanel(
            renderer,
            font,
            buttons, 
            textDisplays,
            std::to_string(obj->GetPosition().x),
            textPositionX,
            textPositionY,
            [obj]() {
                std::lock_guard<std::mutex> lock(sceneObjectMutex);
                //std::cout << "Changing Object X Position!" << std::endl;
                if (obj != nullptr) {
                    //std::cout << "Before -> " << obj->GetPosition().x << std::endl;
                    obj->GetPosition().x += 1;
                    //std::cout << "After --> " << obj->GetPosition().x << std::endl;
                }
            },
            [obj]() {
                std::lock_guard<std::mutex> lock(sceneObjectMutex);
                if (obj != nullptr) {
                    obj->GetPosition().x -= 1;
                }
            }
        );
        CreatePositionPanel(
            renderer,
            font,
            buttons, 
            textDisplays,
            std::to_string(obj->GetPosition().y),
            textPositionX + 250,
            textPositionY,
            [obj]() {
                std::lock_guard<std::mutex> lock(sceneObjectMutex);
                if (obj != nullptr) {
                    obj->GetPosition().y += 1;
                }
            },
            [obj]() {
                std::lock_guard<std::mutex> lock(sceneObjectMutex);
                if (obj != nullptr) {
                    obj->GetPosition().y -= 1;
                }
            }
        );
        CreatePositionPanel(
            renderer,
            font,
            buttons, 
            textDisplays,
            std::to_string(obj->GetPosition().z),
            textPositionX + 500,
            textPositionY,
            [obj]() {
                std::lock_guard<std::mutex> lock(sceneObjectMutex);
                if (obj != nullptr) {
                    obj->GetPosition().z += 1;
                }
            },
            [obj]() {
                std::lock_guard<std::mutex> lock(sceneObjectMutex);
                if (obj != nullptr) {
                    obj->GetPosition().z -= 1;
                }
            }
        );
    }
    //====================================

    SDL_Event event;
    bool focused = false;
    while (running && engineRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }

            // WINDOW EVENTS
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.windowID != SDL_GetWindowID(window))
                    continue;

                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    running = false;
                } else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                    focused = true;
                } else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    focused = false;
                }
            }

            // MOUSE HOVER EVENTS
            if (event.type == SDL_MOUSEMOTION) {
                if (!focused) continue;
                if (event.motion.windowID != SDL_GetWindowID(window))
                    continue;

                for (Inspector_UI::Button* button : buttons) {
                    if (button) {
                        button->hovered = Inspector_UI::PointInRect(
                            event.motion.x,
                            event.motion.y,
                            button->rect
                        );
                    }
                }
            }

            // MOUSE CLICK EVENTS
            if (event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT) {

                if (!focused) continue;
                if (event.button.windowID != SDL_GetWindowID(window))
                    continue;

                for (Inspector_UI::Button* button : buttons) {
                    if (button &&
                        Inspector_UI::PointInRect(
                            event.button.x,
                            event.button.y,
                            button->rect)) {
                        button->onClick();
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // draw btns
        for (Inspector_UI::Button* button : buttons) {
            if (button != nullptr) {
                // Button background
                SDL_SetRenderDrawColor(renderer, button->hovered ? 100 : 70, 70, 70, 255);
                SDL_RenderFillRect(renderer, &button->rect);

                // Button border
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
                SDL_RenderDrawRect(renderer, &button->rect);

                // Button text
                SDL_RenderCopy(renderer, button->textTexture, nullptr, &button->textRect);
            }
        }

        // draw text displays
        size_t objectIndex = 0;
        int j = 0;
        for (Inspector_UI::TextField* textDisplay : textDisplays) {
            if (textDisplay != nullptr) {
                // update displayed text
                if (activeScene != nullptr && activeScene->GetSceneObjects() != nullptr) {
                    std::string ntext;

                    // anonymous scope for the mutex
                    {
                        std::lock_guard<std::mutex> lock(sceneObjectMutex);

                        auto* obj = activeScene->GetSceneObjects()->at(objectIndex);
                        if (!obj) continue;

                        if (j == 0) ntext = std::to_string(obj->GetPosition().x);
                        else if (j == 1) ntext = std::to_string(obj->GetPosition().y);
                        else if (j == 2) ntext = std::to_string(obj->GetPosition().z);
                    }
                    
                    if (ntext != textDisplay->text) {
                        textDisplay->text = ntext;
                        UpdateTextField(
                            textDisplay,
                            renderer,
                            font,
                            {255,255,255,255}
                        );
                    }
                }

                // background
                SDL_RenderFillRect(renderer, &textDisplay->rect);

                // border
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
                SDL_RenderDrawRect(renderer, &textDisplay->rect);

                // text
                SDL_RenderCopy(renderer, textDisplay->textTexture, nullptr, &textDisplay->textRect);
            }

            j = ++j % 3;
            objectIndex = (j % 3 == 0) ? ++objectIndex : objectIndex;
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    for (Inspector_UI::Button* button : buttons) {
        if (button != nullptr) {
            // Button Texture clean-up
            SDL_DestroyTexture(button->textTexture);
            delete button;
        }
    }

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