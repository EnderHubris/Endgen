#include <scene.hpp>

EndgenScene::EndgenScene(int w, int h, SDL_Renderer* rend, SDL_Texture* text) {
    WIDTH = w;
    HEIGHT = h;

    renderer = rend;
    texture = text;
    
    MainCamera = &Camera::Instance();
}

void EndgenScene::Render() {
    if (renderer == nullptr || texture == nullptr) return;

    // Lock texture to write pixels
    void* pixels = nullptr;
    int pitch = 0;

    SDL_LockTexture(texture, nullptr, &pixels, &pitch); // SEG FAULT

    Uint32* pixelBuffer = static_cast<Uint32*>(pixels);
    int pitchPixels = pitch / sizeof(Uint32);

    // Draw pixels (simple gradient)
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            Uint8 r = static_cast<Uint8>(x % 256);
            Uint8 g = static_cast<Uint8>(y % 256);
            Uint8 b = 128;

            pixelBuffer[y * pitchPixels + x] =
                (0xFF << 24) | (r << 16) | (g << 8) | b; // ARGB
        }
    }

    SDL_UnlockTexture(texture);

    // Render to screen
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

EndgenScene::~EndgenScene() {}