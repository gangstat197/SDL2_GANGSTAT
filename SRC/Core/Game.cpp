#include <Core/Game.h>
#include <Core/Renderer.h>
#include <Utils/Input.h>

Game::Game() : window(nullptr), renderer(nullptr), isRunning(false) {}

Game::~Game() {
    Clean();
}

bool Game::Init(const char* title, int width, int height) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Create the window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Create the renderer
    renderer = new Renderer(window, width, height);
    if (renderer == nullptr) {
        SDL_Log("Failed to initialize renderer");
        return false;
    }

    isRunning = true;
    return true;
}

void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

void Game::Update() {
    // Update game logic here
}

void Game::Render() {
    // Clear the renderer
    renderer->Clear();

    backgroundTexture = renderer->LoadTexture("ASSETS/Images/Background_BottomBikini.jpg");
    renderer->RenderTexture(backgroundTexture, 0, 0);

    SDL_Texture* texture = renderer->LoadTexture("ASSETS/Images/Test_Pokemon.png");
    renderer->RenderTexture(texture, 800 / 2, 600 / 2);

    // Present the renderer
    renderer->Present();

    // Clean up resources
    renderer->DestroyTexture(texture); // Ensure texture is destroyed after use
    texture = nullptr;

    renderer->DestroyTexture(backgroundTexture);
    backgroundTexture = nullptr;
}

void Game::Clean() {
    // Clean up resources
    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

bool Game::IsRunning() const {
    return isRunning;
}
