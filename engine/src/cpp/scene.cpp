#include <scene.hpp>

EndgenScene::EndgenScene(int w, int h, SDL_Renderer* rend, SDL_Texture* text) {
    WIDTH = w;
    HEIGHT = h;

    renderer = rend;
    texture = text;
    
    MainCamera = &Camera::Instance();
}

void EndgenScene::SetCamera(Vector3 camPos) {
    if (MainCamera != nullptr) {
        MainCamera->position = camPos;
    }
}

std::vector<WorldObject*>* EndgenScene::GetSceneObjects() { return &sceneObjects; };

void EndgenScene::Render() {
    if (renderer == nullptr || texture == nullptr) return;

    // pointer to locked pixels
    void* pixels = nullptr;
    int bytesPerRow = 0;
    
    // Lock texture to write pixels
    int lockTexture = SDL_LockTexture(texture, nullptr, &pixels, &bytesPerRow);
    if (lockTexture == 0) {
        Uint32* pixelBuffer = static_cast<Uint32*>(pixels); // buffer we will draw to
        int pixelsPerRow = bytesPerRow / sizeof(Uint32);    // roughly evaluates to WIDTH
        
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                int row = y * pixelsPerRow;
                int col = x;

                Vector3 rayDir = RayFromCamera(x,y);
                Uint32 color = RaycastScene(rayDir);
                pixelBuffer[row + col] = color;

                // pixelBuffer[row + col] = VOID_COLOR;
            }
        }
    
        // unlock texture so it can be used
        SDL_UnlockTexture(texture);
    
        // Render to screen
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    } else {
        std::cerr << "[-] Error Locking Texture! [" << lockTexture << "]" << std::endl;
    }
}

Vector3 EndgenScene::RayFromCamera(int x, int y) {
    if (MainCamera == nullptr) return v3_zero;
    
    // normalize coords
    float ndcX = (x + 0.5f) / float(WIDTH);
    float ndcY = (y + 0.5f) / float(HEIGHT);

    // screen-space coords
    float screenX = (2.f * ndcX - 1.f);
    float screenY = (1.f - 2.f * ndcY);

    float aspectRatio = float(WIDTH) / float(HEIGHT);
    float scale = tan(MainCamera->FovRad() / 2.f);

    // ray in world-space
    Vector3 rayDir = MainCamera->forward
                   + MainCamera->right * (screenX * aspectRatio * scale)
                   + MainCamera->up * (screenY * scale);

    return rayDir.Normalize();
}

Uint32 EndgenScene::RaycastScene(const Vector3& rayDir)
{
    if (MainCamera == nullptr) return -1;

    Vector3& camPos = MainCamera->position;

    // simple plane at z = 0
    float t = -camPos.y / rayDir.y;  // intersection t along ray
    if (t <= 0.f) {
        return VOID_COLOR; // background
    }

    Vector3 hitPos = camPos + rayDir * t;

    for (WorldObject* obj : sceneObjects) {
        if (obj != nullptr) {
            if (obj->ContainsPoint(hitPos)) {
                return obj->GetColor();
            }
        }
    }

    // // simple checkerboard coloring
    // int checkX = int(floor(hitPos.x)) & 1;
    // int checkY = int(floor(hitPos.y)) & 1;

    // Uint32 color;
    // if (checkX ^ checkY) color = 0xFFFFFFFF;    // white
    // else color = 0xFF000000;                    // black

    return VOID_COLOR;
}

EndgenScene::~EndgenScene() {
    for (WorldObject* obj : sceneObjects) {
        if (obj != nullptr) {
            delete obj;
        }
    }
}