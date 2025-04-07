#include <states/PlayingState.h>
#include <iostream>

PlayingState::PlayingState(Renderer* renderer, AssetManager* assetManager, InputSystem* input)
    : State(renderer, assetManager, input) {
}

PlayingState::~PlayingState() {
}

void PlayingState::Init() {
    std::cout << "PlayingState initialized\n";
    
    InitializeBackground("background", 800, 800, 30);
    InitializeCursor("mouse_cursor");

    // Intialize player


    // Load enemy texture
    SDL_Texture* enemy_texture = m_assetManager->LoadTexture("enemy", "assets/images/test_bacteria.png", m_renderer->GetSDLRenderer());
    enemy_texture = m_assetManager->ScaleTexture("enemy", *enemy_texture, m_renderer->GetSDLRenderer(), 0.25);
 
    // // Create a test enemy
    m_test_enemy = new Enemy(m_renderer, m_assetManager, "enemy", ColliderType::POLYGON);

    int width = m_test_enemy->GetWidth();
    int height = m_test_enemy->GetHeight();

    std::vector<Vector2D> points = {
        Vector2D(-width/2, -height/2),
        Vector2D(width/2, -height/2),
        Vector2D(width/2, height/2),
        Vector2D(-width/2, height/2),
    };

    m_test_enemy->GetColliderComponent()->SetPolygonCollider(4, &points);
    m_test_enemy->SetPosition(Vector2D(200, 200));
    m_test_enemy->SetSpeed(10); 
    m_test_enemy->SetRotationSpeed(10);
    m_test_enemy->SetMovementPattern(MovementPattern::STRAIGHT);
}

void PlayingState::HandleEvents() {
    m_input->HandleEvents();
}

void PlayingState::Update() {
    float deltaTime = 0.016f; // FIXED DELTA TIME - 60 FPS
    // TODO: Implement a proper timer for deltaTime
    
    m_test_enemy->Update(deltaTime);
    std::cout << "Collision: " << m_test_enemy->GetColliderComponent()->CheckCollision() << std::endl;
    m_cursor->Update();
}

void PlayingState::Render() {
    m_backgroundManager->RenderBackground();
    m_backgroundManager->InfiniteBackground();
    
    m_test_enemy->Render();
    
    m_cursor->Render();
}

void PlayingState::Cleanup() {
    std::cout << "PlayingState cleaned up\n";
}
