#include <core/Game.h>
#include <core/Renderer.h>

#include <systems/InputSystem.h>
#include <utils/Vector2D.h>
#include <utils/SpriteSheet.h>

#include <iostream>

Game::Game() : input(nullptr), renderer(nullptr), isRunning(false) {}

Game::~Game() {
    Clean();
}

bool Game::Init(const char* title, int width, int height) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Create graphic 
    renderer = new Renderer(title, width, height);

    // Create input manager
    input = new InputSystem();

    // --------------Load Game Assets
    // Load background texture
    backgroundTexture = renderer->LoadTexture("assets/images/LightBlue_Background.png");
    backgroundTexture = renderer->ScaleTexture(*backgroundTexture, 900, 800); 
    if (backgroundTexture == nullptr) {
        std::cout << "Failed to load background texture\n";
        return false;
    }

    // Set frame count to 0
    frameCount = 0;

    // Set running state 
    isRunning = true;
    return true;
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

    // Render the background texture
    renderer->RenderTexture(backgroundTexture, 0, 0);

    // Test sprite sheet
    // SpriteSheet* spriteSheet = new SpriteSheet(renderer->GetSDLRenderer(), "ASSETS/Images/1_Pink_Monster/Pink_Monster_Attack1_4.png", 1, 4);
    // renderer->RenderSprite(spriteSheet, 100, 100, frameCount / 16);
    // frameCount++;

    // if (frameCount / 16 >= spriteSheet->GetClips().size()) {
    // 	frameCount = 0;
    // }

    // Present the renderer
    renderer->Present();

    // // Destroy the sprite sheet texture
    // spriteSheet->DestroyTexture();
    // delete spriteSheet;
    // spriteSheet = nullptr;
}

void Game::Clean() {
    if (backgroundTexture) {
        renderer->DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
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

void Game::Update() {
    // std::cout << "Updating game state\n";
}

bool Game::IsRunning() {
    return isRunning;
}