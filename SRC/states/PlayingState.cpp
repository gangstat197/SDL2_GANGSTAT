#include <states/PlayingState.h>
#include <iostream>

PlayingState::PlayingState(Renderer* renderer, AssetManager* assetManager, InputSystem* input)
    : State(renderer, assetManager, input) {
}

PlayingState::~PlayingState() {
    // Base destructor will handle cleanup of common resources
}

void PlayingState::Init() {
    std::cout << "PlayingState initialized\n";
    
    // Initialize common resources using base class helpers
    InitializeBackground("background", 800, 800, 100);
    InitializeCursor("mouse_cursor");

    // Initialize test enemy
    SDL_Texture* enemy_texture = m_assetManager->LoadTexture("enemy", "assets/images/test_bacteria.png", m_renderer->GetSDLRenderer());
    enemy_texture = m_assetManager->ScaleTexture("enemy", *enemy_texture, m_renderer->GetSDLRenderer(), 0.25);

    m_test_enemy = new Enemy(m_renderer, m_assetManager, "enemy");
    m_test_enemy->SetPosition(Vector2D(400, -20));
    m_test_enemy->SetSpeed(40);
    m_test_enemy->SetRotationSpeed(10);
    m_test_enemy->SetMovementPattern(MovementPattern::STRAIGHT);
}

void PlayingState::HandleEvents() {
    m_input->HandleEvents();
}

void PlayingState::Update() {
    float deltaTime = 0.016f; // Approximately 60 FPS
    m_test_enemy->Update(deltaTime);
    m_cursor->Update();
}

void PlayingState::Render() {
    // Render background
    m_backgroundManager->RenderBackground();
    m_backgroundManager->InfiniteBackground();
    
    // Render game elements here
    m_test_enemy->Render();
    
    // Render cursor on top
    m_cursor->Render();
}

void PlayingState::Cleanup() {
    std::cout << "PlayingState cleaned up\n";
    // Base class destructor will handle cleanup of common resources
}
