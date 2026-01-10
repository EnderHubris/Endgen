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

size_t EndgenScene::ObjectCount() const { return sceneObjects.size(); }

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

bool RenderMath::RayIntersectsAABB(
    const Vector3& origin,
    const Vector3& dir,
    const Vector3& min,
    const Vector3& max,
    float& tOut
) {
    float tmin = 0.0f;
    float tmax = 1e30f;

    // iterate over all components of a vector3
    for (int i = 0; i < 3; ++i) {
        float o = origin[i];
        float d = dir[i];

        if (std::abs(d) < 1e-6f) {
            if (o < min[i] || o > max[i])
                return false;
        } else {
            float invD = 1.0f / d;
            float t1 = (min[i] - o) * invD;
            float t2 = (max[i] - o) * invD;

            if (t1 > t2) std::swap(t1, t2);

            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);

            if (tmin > tmax)
                return false;
        }
    }

    tOut = tmin;
    return true;
}

Uint32 EndgenScene::RaycastScene(const Vector3& rayDir)
{
    if (MainCamera == nullptr) return -1;

    Vector3& camPos = MainCamera->position;

    float closestT = 1e30f;
    WorldObject* closestObj = nullptr;

    // anonymous block for mutex
    {
        std::lock_guard<std::mutex> lock(sceneObjectMutex);

        for (WorldObject* obj : sceneObjects) {
            if (!obj) continue;

            float t;
            // object with smallest t value is closet object
            if (RenderMath::RayIntersectsAABB(
                    camPos,
                    rayDir,
                    obj->Min(),
                    obj->Max(),
                    t
                )) {
                if (t > 0.0f && t < closestT) {
                    closestT = t;
                    closestObj = obj;
                }
            }
        }
    }

    if (closestObj)
        return closestObj->GetColor();

    return VOID_COLOR;
}

EndgenScene::~EndgenScene() {
    for (WorldObject* obj : sceneObjects) {
        if (obj != nullptr) {
            delete obj;
        }
    }
}