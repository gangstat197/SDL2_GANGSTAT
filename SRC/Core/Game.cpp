#include <core/Game.h>
#include <core/Renderer.h>

#include <systems/InputSystem.h>
#include <systems/Cursor.h>
#include <utils/Vector2D.h>
#include <utils/SpriteSheet.h>

#include <iostream>

Game::Game() : 
    input(nullptr), 
    renderer(nullptr), 
    isRunning(false), 
    assetManager(nullptr), 
    backgroundManager(nullptr),
    cursor(nullptr) {}

Game::~Game() {
    Clean();
}

bool Game::Init(const char* title, int width, int height) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Disable the default system cursor
    SDL_ShowCursor(SDL_DISABLE);

    // Create graphic 
    renderer = new Renderer(title, width, height);

    // Create input manager
    input = new InputSystem();

    // Use singleton instance of AssetManager
    assetManager = &AssetManager::Instance();
    LoadAssets();
    
    backgroundManager = new BackgroundManager(renderer->GetSDLRenderer(), assetManager->GetTexture("background"), width, height);
    backgroundManager->SetScrollSpeed(100); // Set scroll speed for the background
    
    cursor = new Cursor(renderer->GetSDLRenderer(), assetManager, input, "mouse_cursor");

    // Set frame count to 0
    frameCount = 0;
    
    // Set running state 
    isRunning = true;
    return true;
}

void Game::LoadAssets() {
    SDL_Texture* backgroundTexture = assetManager->LoadTexture("background", "assets/images/WhiteGrid_Background.png", renderer->GetSDLRenderer());
    backgroundTexture = assetManager->ScaleTexture(*backgroundTexture, renderer->GetSDLRenderer(), 600, 800); 

    if (backgroundTexture == nullptr) {
        std::cout << "Failed to load background texture\n";
        return;
    }

}

void Game::HandleEvents() {
    input->HandleEvents();
    
    if (input->IsQuitRequested()) {
        std::cout << "Quit Game!";
        isRunning = false;
        return;
    }
}

void Game::Render() {
    // std::cout << "Rendering\n";
    // Clear the renderer
    renderer->Clear();

    backgroundManager->RenderBackground(); // Render the background
    backgroundManager->InfiniteBackground(); // Infinite scrolling background

    cursor->Render();

    // Present the renderer
    renderer->Present();

    // // Destroy the sprite sheet texture
    // spriteSheet->DestroyTexture();
    // delete spriteSheet;
    // spriteSheet = nullptr;
}

void Game::Clean() {
    assetManager->UnloadAll(); // Unload all textures

    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }

    if (input) {
        delete input;
        input = nullptr;
    }

    delete cursor;
    cursor = nullptr;

    SDL_Quit();
}

void Game::Update() {
    // std::cout << "Updating game state\n";
    cursor->Update();
}

bool Game::IsRunning() {
    return isRunning;
}