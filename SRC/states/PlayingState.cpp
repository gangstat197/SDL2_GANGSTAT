#include <states/PlayingState.h>
#include <iostream>

PlayingState::PlayingState(Renderer* renderer, AssetManager* assetManager, InputSystem* input)
    : State(renderer, assetManager, input) {
}

PlayingState::~PlayingState() {
}

void PlayingState::InitPlayer() {
    // Intialize player
    m_player = new Player(m_renderer, m_assetManager, m_input, "player");

    int width = m_player->GetWidth();
    int height = m_player->GetHeight();

    std::vector<Vector2D> playerColliderPoints = {
        Vector2D(-width/2, -height/2),
        Vector2D(width/2, -height/2),
        Vector2D(width/2, height/2),
        Vector2D(-width/2, height/2),
    };

    m_player->GetColliderComponent()->SetPolygonCollider(4, &playerColliderPoints);
}

void PlayingState::Init() {
    std::cout << "PlayingState initialized\n";
    
    
    InitializeBackground("background", 800, 800, 30);

    InitPlayer();

    // Load enemy texture
    // SDL_Texture* enemy_texture = m_assetManager->LoadTexture("enemy", "assets/images/test_bacteria.png", m_renderer->GetSDLRenderer());
    // enemy_texture = m_assetManager->ScaleTexture("enemy", *enemy_texture, m_renderer->GetSDLRenderer(), 0.25);
 
    // // // Create a test enemy
    // m_test_enemy = new Enemy(m_renderer, m_assetManager, "enemy", ColliderType::POLYGON);

    // int width = m_test_enemy->GetWidth();
    // int height = m_test_enemy->GetHeight();

    // std::vector<Vector2D> points = {
    //     Vector2D(-width/2, -height/2),
    //     Vector2D(width/2, -height/2),
    //     Vector2D(width/2, height/2),
    //     Vector2D(-width/2, height/2),
    // };

    // m_test_enemy->GetColliderComponent()->SetPolygonCollider(4, &points);
    // m_test_enemy->SetPosition(Vector2D(200, 200));
    // m_test_enemy->SetSpeed(20); 
    // m_test_enemy->SetRotationSpeed(10);
    // m_test_enemy->SetMovementPattern(MovementPattern::CURVE);

    // Create a new enemy
    m_test_enemy = new Enemy(m_renderer, m_assetManager, "enemy_round", ColliderType::CIRCLE);

    int width = m_test_enemy->GetWidth(); 
    
    m_test_enemy->GetColliderComponent()->SetCircleCollider(width/2);    
    m_test_enemy->SetPosition(Vector2D(400, 400));
    m_test_enemy->SetSpeed(20);
    m_test_enemy->SetRotationSpeed(10);
    m_test_enemy->SetMovementPattern(MovementPattern::STRAIGHT);
}

void PlayingState::HandleEvents() {
    m_input->HandleEvents();
}

void PlayingState::Update() {
    float deltaTime = 0.016f; 
    
    m_test_enemy->Update(deltaTime);
    m_player->Update(deltaTime);
    std::cout << "Collision: " << m_test_enemy->GetColliderComponent()->CheckCollision(m_player->GetColliderComponent()) << std::endl;
}

void PlayingState::Render() {
    m_backgroundManager->RenderBackground();
    m_backgroundManager->InfiniteBackground();
    
    m_test_enemy->Render();
    m_player->Render();
}

void PlayingState::Cleanup() {
    std::cout << "PlayingState cleaned up\n";
}
