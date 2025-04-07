#include <core/Game.h>
#include <core/Renderer.h>

#include <systems/InputSystem.h>
#include <utils/Vector2D.h>
#include <utils/SpriteSheet.h>

#include <iostream>

Game::Game() : 
    input(nullptr), 
    renderer(nullptr), 
    isRunning(false), 
    assetManager(nullptr),
    stateManager(nullptr) {}

Game::~Game() {
    Clean();
}

bool Game::Init(const char* title, int width, int height) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }


    SDL_ShowCursor(SDL_DISABLE);
    
    // Core components
    renderer = new Renderer(title, width, height);
    input = new InputSystem();
    assetManager = &AssetManager::Instance();
    LoadAssets();

    // State manager
    
    stateManager = new GameStateManager();
    InitStates();
    
    stateManager->SwitchState(GameStates::MENU);

    frameCount = 0;
    
    isRunning = true;
    return true;
}

void Game::LoadAssets() {
    // Load Background texture
    SDL_Texture* backgroundTexture = assetManager->LoadTexture("background", "assets/images/WhiteGrid_Background.png", renderer->GetSDLRenderer());

    if (backgroundTexture == nullptr) {
        std::cout << "Failed to load background texture\n";
        return;
    }

    // Load Heading texture
    SDL_Texture* headingTexture = assetManager->LoadTexture("heading", "assets/images/MicroDash_Hero.png", renderer->GetSDLRenderer());

    if (headingTexture == nullptr) {
        std::cout << "Failed to load heading texture\n";
        return;
    }

    headingTexture = assetManager->ScaleTexture("heading", *headingTexture, renderer->GetSDLRenderer(), 0.25);
    
    // Load cursor texture
    SDL_Texture* cursorTexture = assetManager->LoadTexture("mouse_cursor", "assets/images/cursor_yellow.png", renderer->GetSDLRenderer());
    
    // Load UI textures
    SDL_Texture* playButtonTexture = assetManager->LoadTexture("button_play", "assets/images/ui/btn_play.png", renderer->GetSDLRenderer());
    playButtonTexture = assetManager->ScaleTexture("button_play", *playButtonTexture, renderer->GetSDLRenderer(), 0.25);
    
    SDL_Texture* optionsButtonTexture = assetManager->LoadTexture("button_options", "assets/images/ui/btn_options.png", renderer->GetSDLRenderer());
    optionsButtonTexture = assetManager->ScaleTexture("button_options", *optionsButtonTexture, renderer->GetSDLRenderer(), 0.25);

    SDL_Texture* exitButtonTexture = assetManager->LoadTexture("button_quit", "assets/images/ui/btn_quit.png", renderer->GetSDLRenderer());
    exitButtonTexture = assetManager->ScaleTexture("button_quit", *exitButtonTexture, renderer->GetSDLRenderer(), 0.25);

    SDL_Texture* debug_position = assetManager->LoadTexture("debug_position", "assets/images/debug/debug_reddot.png", renderer->GetSDLRenderer());
    debug_position = assetManager->ScaleTexture("debug_position", *debug_position, renderer->GetSDLRenderer(), 0.5);

}

void Game::InitStates() {
    // Initialize states 
    menuState = std::make_shared<MenuState>(renderer, assetManager, input);
    playingState = std::make_shared<PlayingState>(renderer, assetManager, input);
    quitState = std::make_shared<QuitState>(renderer, assetManager, input, isRunning);
    
    // Register states 
    stateManager->RegisterState(GameStates::MENU, menuState);
    stateManager->RegisterState(GameStates::PLAYING, playingState);
    stateManager->RegisterState(GameStates::QUIT, quitState);
}
    
void Game::HandleEvents() {
    // Process input events
    input->HandleEvents();
    
    // Let the current state handle events
    stateManager->HandleEvents();
    
    // Check if a quit was requested
    if (input->IsQuitRequested()) {
        stateManager->SwitchState(GameStates::QUIT);
    }
}

void Game::Render() {
    renderer->Clear();
    
    stateManager->Render();
    
    renderer->Present();
}

void Game::Update() {
    stateManager->Update();
}

void Game::Clean() {
    assetManager->UnloadAll(); 

    if (stateManager) {
        delete stateManager;
        stateManager = nullptr;
    }

    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }

    if (input) {
        delete input;
        input = nullptr;
    }
    
    SDL_Quit();
}


bool Game::IsRunning() {
    return isRunning;
}