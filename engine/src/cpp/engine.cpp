#include <engine.hpp>

EndgenEngine::EndgenEngine(int w, int h): WIDTH(w), HEIGHT(h) {
    std::cout << "[*] Starting Engine. . ." << std::endl;
    if (!Init()) return;

    scene = new EndgenScene(w, h, renderer, texture);
    if (scene != nullptr) {
        scene->SetCamera(Vector3(0,4,0));
        
        // populate the World Space with Objects
        std::vector<WorldObject>* sceneObjs = scene->GetSceneObjects();
        if (sceneObjs != nullptr) {
            *sceneObjs = {
                Ground(25, 25),
                Cube(5, 5, 5, Vector3(0,4,15), 0xFF0000FF)
            };
        }
    }

    Run();
}

bool EndgenEngine::Init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "[-] SDL failed to initialize: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window
    window = SDL_CreateWindow(
        "Game Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "[-] Window creation failed: " << SDL_GetError() << std::endl;
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
        std::cerr << "[-] Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // Create streaming texture (pixel buffer)
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

    return true;
}

void EndgenEngine::GetInput() {
    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    int sensitivity = 5;
    float dx = 0.01f;
    float dy = 0.01f;

    // Look Left and Right
    if (keys[SDL_SCANCODE_A]) {
        // A is being held
        Camera::Instance().yRot += dy * sensitivity;
    } else if (keys[SDL_SCANCODE_D]) {
        // D is being held
        Camera::Instance().yRot -= dy * sensitivity;
    }

    // Look Up and Down
    if (keys[SDL_SCANCODE_W]) {
        // W is being held
        Camera::Instance().xRot += dx * sensitivity;
    } else if (keys[SDL_SCANCODE_S]) {
        // S is being held
        Camera::Instance().xRot -= dx * sensitivity;
    }
}

void EndgenEngine::Run() {
    bool running = true;
    SDL_Event event;
    std::cout << "[*] Rendering Scene. . ." << std::endl;

    while (running)
    {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        GetInput();

        Camera::Instance().Update();

        RenderScene();

        // wait in milliseconds to not over utilize the CPU
        SDL_Delay(16); // --> 1000 / 16 = 62.5 ~= 62-63 FPS
    }
}

void EndgenEngine::RenderScene() {
    if (scene != nullptr)
        scene->Render();
}

EndgenEngine::~EndgenEngine() {
    std::cout << "[*] Closing Engine. . ." << std::endl;

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

    if (scene != nullptr) {
        delete scene;
    }
    std::cout << " ├─ Closed Scene Successfully!" << std::endl;

    SDL_Quit();

    std::cout << "[+] Engine Closed Successfully!" << std::endl;
}