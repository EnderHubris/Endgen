#include <engine.hpp>

SDL_Window* SDL_Helper::Create_Window(const char* WINDOW_NAME, int x, int y, int width, int height) {
    return SDL_CreateWindow(
        WINDOW_NAME,
        x,
        y,
        width,
        height,
        SDL_WINDOW_SHOWN
    );
};
SDL_Renderer* SDL_Helper::Create_Renderer(SDL_Window* window, bool useGPU) {
    return SDL_CreateRenderer(
        window,
        -1,
        useGPU ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE
    );
};
SDL_Texture* SDL_Helper::Create_Texture(SDL_Renderer* renderer, int width, int height) {
    return SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );
};
TTF_Font* SDL_Helper::Create_Font( const char* font_path, int fontSize) {
    return TTF_OpenFont(
        font_path, fontSize
    );
};
//====================================================================

EndgenEngine::EndgenEngine(bool inspectorMode, int w, int h): WIDTH(w), HEIGHT(h) {
    std::cout << "[*] Starting Engine. . ." << std::endl;

    inspector = nullptr;
    if (inspectorMode && inspector == nullptr) {
        inspector = new Inspector();
    }

    if (!Init()) {
        std::cerr << "[-] Engine Initialize failed" << std::endl;
        return;
    }

    scene = new EndgenScene(w, h, renderers[0], textures[0]);
    if (scene != nullptr) {
        scene->SetCamera(Vector3(0,4,0));
        
        // populate the World Space with Objects
        std::vector<WorldObject*>* sceneObjs = scene->GetSceneObjects();
        if (sceneObjs != nullptr) {
            *sceneObjs = {
                new Ground(25, 25),
                new Cube(6, 6, 6, Vector3(0,2,25), 0xFF0000FF)
            };
        }
    }

    if (inspector != nullptr) {
        auto inspectorMain = [](Inspector* inspector) {
            if (inspector != nullptr) inspector->StartUp();
        };

        tInspector = std::thread(inspectorMain, inspector);
    }

    Run();
}

bool EndgenEngine::Init() {
    // Initialize SDL Components (only appears once)
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "[-] SDL failed to initialize: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() != 0) {
        std::cerr << "[-] TTF_Init failed" << std::endl;
        return false;
    }

    // Create windows
    windows.push_back(SDL_Helper::Create_Window(
        "Game Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT
    ));
    if (inspector != nullptr) {
        windows.push_back(SDL_Helper::Create_Window(
            "Inspector Window",
            800,
            600,
            WIDTH,
            HEIGHT
        ));
    }

    // check window creation
    for (SDL_Window* window : windows) {
        if (!window) {
            std::cerr << "[-] Window creation failed: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }
    }

    // Create renderers

    for (SDL_Window* window : windows) {
        SDL_Renderer* rend = SDL_Helper::Create_Renderer(
            window,
            (SDL_GetWindowID(window) == SDL_GetWindowID(windows[0]))
        );
        SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

        renderers.push_back(rend);
        if (!rend) {
            std::cerr << "[-] Renderer creation failed: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }
    }

    // Create streaming textures (pixel buffer)
    for (size_t i = 0; i < renderers.size(); ++i) {
        SDL_Window* window = windows[i];
        SDL_Renderer* renderer = renderers[i];

        SDL_Texture* texture = SDL_Helper::Create_Texture(
            renderer,
            WIDTH,
            HEIGHT
        );
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        textures.push_back(texture);

        if (!texture) {
            std::cerr << "[-] Texture creation failed: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }
    }

    // initialize font used by inspector window
    font = SDL_Helper::Create_Font();
    if (!font) {
        std::cerr << "[-] Failed to load Font" << std::endl;
        return false;
    }

    // attach inspector components
    if (inspector != nullptr) {
        std::cerr << "[*] Connecting Inspector Components. . ." << std::endl;
        inspector->SetWindow(windows[1]);
        inspector->SetRenderer(renderers[1]);
        inspector->SetTexture(textures[1]);
        inspector->SetFont(font);
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
    bool focused = true;
    
    SDL_Event event;
    std::cout << "[*] Rendering Scene. . ." << std::endl;

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT) {
                Uint32 id = event.window.windowID;
                bool closeWindow = event.window.event == SDL_WINDOWEVENT_CLOSE;

                if (focused && id == SDL_GetWindowID(windows[0])) {
                    // ENGINE WINDOW
                    if (closeWindow) {
                        running = engineRunning = false;
                    }
                    
                    if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                        focused = true;
                    } else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                        focused = false;
                    }
                }
            }
        }

        GetInput();

        Camera::Instance().Update();

        RenderScene();

        // wait in milliseconds to not over utilize the CPU
        SDL_Delay(16); // --> 1000 / 16 = 62.5 ~= 62-63 FPS
    }
    std::cout << "[*] Rendering Loop Closing. . ." << std::endl;
}

void EndgenEngine::RenderScene() {
    // auto-unlock on end of function
    std::lock_guard<std::mutex> lock(sceneObjectMutex);
    if (scene != nullptr)
        scene->Render();
}

EndgenEngine::~EndgenEngine() {
    // ensure the inspector window has been properly closed
    if (tInspector.joinable()) {
        std::cout << "[*] Ensuring Inspector Thread Completion. . ." << std::endl;
        tInspector.join();
    }

    if (inspector != nullptr) {
        delete inspector;
    }

    std::cout << "[*] Closing Engine. . ." << std::endl;

    for (SDL_Window* window : windows) {
        if (window != nullptr) {
            SDL_DestroyWindow(window);
        }
    }
    std::cout << " ├─ Closed Window(s) Successfully!" << std::endl;

    for (SDL_Texture* texture : textures) {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
        }
    }
    std::cout << " ├─ Closed Texture(s) Successfully!" << std::endl;

    for (SDL_Renderer* renderer : renderers) {
        if (renderer != nullptr) {
            SDL_DestroyRenderer(renderer);
        }
    }
    std::cout << " ├─ Closed Renderer(s) Successfully!" << std::endl;

    if (scene != nullptr) {
        delete scene;
    }
    std::cout << " ├─ Closed Scene Successfully!" << std::endl;

    TTF_Quit();
    SDL_Quit();

    std::cout << "[+] Engine Closed Successfully!\n\n";
}