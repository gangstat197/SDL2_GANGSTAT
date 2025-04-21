#include <states/PlayingState.h>
#include <iostream>
#include <utils/GameSettings.h>

PlayingState::PlayingState(Renderer* renderer, AssetManager* assetManager, InputSystem* input, SoundManager* soundManager)
    : State(renderer, assetManager, input, soundManager),
      m_player(nullptr),
      m_enemySpawner(nullptr),
      m_deltaTime(0.016f) {
}

PlayingState::~PlayingState() {
    if (m_player) {
        delete m_player;
        m_player = nullptr;
    }
    
    if (m_enemySpawner) {
        delete m_enemySpawner;
        m_enemySpawner = nullptr;
    }
}

void PlayingState::InitPlayer() {
    m_player = new Player(m_renderer, m_assetManager, m_input, m_soundManager, "player");

    int width = m_player->GetWidth();
    int height = m_player->GetHeight();

    std::vector<Vector2D> playerColliderPoints = {
        Vector2D(-width/2, -height/2),
        Vector2D(width/2, -height/2),
        Vector2D(width/2, height/2),
        Vector2D(-width/2, height/2),
    };

    m_player->GetColliderComponent()->SetPolygonCollider(4, playerColliderPoints);
    m_player->SetPosition(GameSettings::SCREEN_WIDTH/2, GameSettings::SCREEN_HEIGHT - 100);
}

void PlayingState::InitEnemySpawner() {
    m_enemySpawner = new EnemySpawner(m_renderer, m_assetManager);
    m_enemySpawner->Initialize();
}

void PlayingState::Init() {
    std::cout << "PlayingState initialized\n";
    
    InitializeBackground("background", 
                         GameSettings::SCREEN_WIDTH, 
                         GameSettings::SCREEN_HEIGHT, 
                         GameSettings::BACKGROUND_SCROLL_SPEED);
    InitPlayer();
    InitEnemySpawner();

    m_gameTimer.Start();
}

void PlayingState::HandleEvents() {
}

void PlayingState::Update() {
    static Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();
    m_deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    
    if (m_deltaTime > 0.1f) {
        m_deltaTime = 0.1f;
    }
    
    m_player->Update(m_deltaTime);
    m_enemySpawner->Update(m_deltaTime);
    
    if (m_enemySpawner->CheckCollisions(m_player)) {
        std::cout << "Player hit by enemy!" << std::endl;
    }

}

void PlayingState::Render() {
    m_backgroundManager->RenderBackground();
    m_backgroundManager->InfiniteBackground();
    
    m_enemySpawner->Render();
    m_player->Render();
}

void PlayingState::Cleanup() {
    std::cout << "PlayingState cleaned up\n";
}
