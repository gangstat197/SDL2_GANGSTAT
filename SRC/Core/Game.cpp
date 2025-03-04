#include <Core/Game.h>
#include <Core/Renderer.h>
#include <Entities/Monster.h>
#include <Utils/Input.h>
#include <Utils/Vector2D.h>
#include <Utils/SpriteSheet.h>

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
    input = new Input();
    isRunning = true;
    return true;
}

void Game::HandleEvents() {
    SDL_Event event;
    Vector2D mousePosition;
    //std::cerr << "Handling Events\n";
    while (SDL_PollEvent(&event)) {
        input->GetMouseState(mousePosition);
        if (event.type == SDL_QUIT) {
            isRunning = false;
            break;
        }

        SDL_Delay(100);
    }
}

void Game::Update() {
    // Update game logic 
}

void Game::Render() {
    //std::cerr << "Rendering\n";
    // Clear the renderer
    renderer->Clear();

    // Create Background
    backgroundTexture = renderer->LoadTexture("ASSETS/Images/Background_BottomBikini.jpg");
    renderer->RenderTexture(backgroundTexture, 0, 0);

    // Test sprite sheet
    SpriteSheet* spriteSheet = new SpriteSheet(renderer->GetSDLRenderer(), "ASSETS/Images/1_Pink_Monster/Pink_Monster_Attack1_4.png", 1, 4);
    renderer->RenderSprite(spriteSheet, 100, 100, 0);

    // Render all game entities
    for (auto& entity : entities) {
        entity->Render(renderer);
    }
    
    // Present the renderer
    renderer->Present();

    // Destroy the sprite sheet texture
    spriteSheet->DestroyTexture();
    delete spriteSheet;
    spriteSheet = nullptr;

    renderer->DestroyTexture(backgroundTexture);
    backgroundTexture = nullptr;
}

void Game::Clean() {
    // Clean up resources
    for (auto& entity : entities) {
		entity->Clean();
		delete entity;
	}
    entities.clear();

    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }
    SDL_Quit();
}

bool Game::IsRunning() const {
    return isRunning;
}
