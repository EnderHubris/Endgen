#include <scene.hpp>

EndgenScene::EndgenScene(int w, int h, SDL_Renderer* rend, SDL_Texture* text) {
    WIDTH = w;
    HEIGHT = h;

    pixelBuffer.resize(WIDTH * HEIGHT);

    renderer = rend;
    texture = text;
    
    MainCamera = &Camera::Instance();

    // populate scene
    /*
    objects = {
        Mesh("objects/cube.obj")
    };
    */
}

void EndgenScene::SetCamera(Vector3 camPos) {
    if (MainCamera != nullptr) {
        MainCamera->position = camPos;
    }
}

SDL_Renderer* EndgenScene::GetRenderer() { return renderer; }
SDL_Texture* EndgenScene::GetTexture() { return texture; }

std::vector<Uint32>& EndgenScene::GetBuffer() { return pixelBuffer; }

void EndgenScene::Render(bool exotic) {
    int pixelsPerRow = WIDTH;

    // fill the background
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int row = y * pixelsPerRow;
            int col = x;

            pixelBuffer[row + col] = VOID_COLOR;
        }
    }

    if (exotic) {
        // populate scene
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        const int numTriangles = 50;
        const float minX = 50.f, maxX = 600.f;
        const float minY = 50.f, maxY = 600.f;
        
        static std::vector<Triangle> triangles;

        if (triangles.empty()) {
            // only run once
            for (int i = 0; i < numTriangles; ++i) {
                Vector2 v0{ minX + static_cast<float>(std::rand()) / RAND_MAX * (maxX - minX),
                            minY + static_cast<float>(std::rand()) / RAND_MAX * (maxY - minY) };

                Vector2 v1{ minX + static_cast<float>(std::rand()) / RAND_MAX * (maxX - minX),
                            minY + static_cast<float>(std::rand()) / RAND_MAX * (maxY - minY) };

                Vector2 v2{ minX + static_cast<float>(std::rand()) / RAND_MAX * (maxX - minX),
                            minY + static_cast<float>(std::rand()) / RAND_MAX * (maxY - minY) };

                triangles.emplace_back(v0, v1, v2);
            }
        }

        for (Triangle& tri : triangles) {
            // compute bounding box
            int minX = std::max( 0.f,                   std::min({ tri.vertices[0].x, tri.vertices[1].x, tri.vertices[2].x }));
            int minY = std::max( 0.f,                   std::min({ tri.vertices[0].y, tri.vertices[1].y, tri.vertices[2].y }));
            int maxX = std::min( (float)(WIDTH-1),      std::max({ tri.vertices[0].x, tri.vertices[1].x, tri.vertices[2].x }));
            int maxY = std::min( (float)(HEIGHT-1),     std::max({ tri.vertices[0].y, tri.vertices[1].y, tri.vertices[2].y }));
        
            for (int y = minY; y <= maxY; ++y) {
                for (int x = minX; x <= maxX; ++x) {
                    if (pixelBuffer[y * pixelsPerRow + x] != VOID_COLOR) continue;
                    if (tri.ContainsPoint(x, y)) {
                        pixelBuffer[y * pixelsPerRow + x] = tri.color;
                    }
                }
            }

            tri.Shift();
        }

        return;
    }

    // minimuze how many pixels we are iterating over
    /*
    for (Mesh& obj : objects) {
        for (Triangle& tri : obj.triangles) {
            // compute bounding boxs?
        
            for (int y = minY; y <= maxY; ++y) {
                for (int x = minX; x <= maxX; ++x) {
                    if (tri.ContainsPoint(x, y)) {
                        pixelBuffer[y * pixelsPerRow + x] = tri.color;
                    }
                }
            }
        }
    }
    */
}

EndgenScene::~EndgenScene() {}