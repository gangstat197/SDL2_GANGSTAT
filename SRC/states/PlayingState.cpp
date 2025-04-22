#include <states/PlayingState.h>
#include <iostream>
#include <utils/GameSettings.h>

PlayingState::PlayingState(Renderer* renderer, AssetManager* assetManager, InputSystem* input, SoundManager* soundManager)
    : State(renderer, assetManager, input, soundManager),
      m_player(nullptr),
      m_enemySpawner(nullptr),
      m_powerUpSpawner(nullptr),
      m_playState(PlayState::PLAYING),  
      m_deltaTime(0.016f),
      m_currentTime(0.0f),
      m_finalScore(0),
      m_saveSettings("data/highscores.dat"),
      m_scoreWasSaved(false) {
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
    
    if (m_powerUpSpawner) {
        delete m_powerUpSpawner;
        m_powerUpSpawner = nullptr;
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
    
    dynamic_cast<Player*>(m_player)->SaveOriginalColliderData();
    
    m_player->SetPosition(GameSettings::SCREEN_WIDTH/2, GameSettings::SCREEN_HEIGHT - 100);
}

void PlayingState::InitEnemySpawner() {
    m_enemySpawner = new EnemySpawner(m_renderer, m_assetManager);
    m_enemySpawner->Initialize();
}

void PlayingState::InitPowerUpSpawner() {
    m_powerUpSpawner = new PowerUpSpawner(m_renderer, m_assetManager);
    m_powerUpSpawner->Initialize();
}

void PlayingState::Init() {
    std::cout << "PlayingState initialized\n";
    
    InitializeBackground("background", 
                         GameSettings::SCREEN_WIDTH, 
                         GameSettings::SCREEN_HEIGHT, 
                         GameSettings::BACKGROUND_SCROLL_SPEED);
    InitPlayer();
    InitEnemySpawner();
    InitPowerUpSpawner();

    m_player->Reset();
    m_player->SetActive(true);
    m_enemySpawner->Reset();
    m_powerUpSpawner->Reset();

    m_playState = PlayState::PLAYING;
    m_gameTimer.Start();
    m_finalScore = 0;
}

void PlayingState::HandleEvents() {
    if (m_playState == PlayState::GAME_OVER) {
        if (m_input->IsKeyJustPressed(SDLK_SPACE) || m_input->IsKeyJustPressed(SDLK_RETURN)) {
            Reset();
        }
        
        if (m_input->IsKeyJustPressed(SDLK_ESCAPE)) {
            RequestStateChange(GameStates::MENU);
        }
    }
}

void PlayingState::Update() {
    static Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();
    m_deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    
    if (m_deltaTime > 0.05f) {
        m_deltaTime = 0.05f;
    }
    
    if (m_playState == PlayState::PLAYING) {
        m_player->Update(m_deltaTime);
        m_enemySpawner->Update(m_deltaTime);
        m_powerUpSpawner->Update(m_deltaTime);
        
        if (m_enemySpawner->CheckCollisions(m_player)) {
            std::cout << "Player hit by enemy!" << std::endl;
        }
        
        if (m_powerUpSpawner->CheckCollisions(m_player)) {
            std::cout << "Player collected a power-up!" << std::endl;
        }
        
        m_currentTime += m_deltaTime;
        std::cout << "Current Time: " << m_currentTime << std::endl;
        if (m_currentTime >= 1.0f) {
            m_finalScore += GameSettings::SCORE_PER_SECOND * m_player->GetPointMultiplier(); 
            m_currentTime -= 1.0f;
        }
        
        CheckGameOver();
    }
}

void PlayingState::Render() {
    m_backgroundManager->RenderBackground();
    m_backgroundManager->InfiniteBackground();
    
    if (m_playState == PlayState::PLAYING) {
        m_powerUpSpawner->Render();
        m_enemySpawner->Render();
        m_player->Render();
        RenderScore();
    } else if (m_playState == PlayState::GAME_OVER) {
        RenderGameOver();
    }
}

void PlayingState::RenderScore() {
    SDL_Texture* scoreTexture = m_assetManager->GetTextTexture(
        "final_score_text",
        "Score: " + std::to_string(m_finalScore),
        {255, 255, 255, 255},
        "game_font",
        28  
    );

    if (scoreTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(scoreTexture, NULL, NULL, &textWidth, &textHeight);
        
        SDL_Rect textRect = {
            GameSettings::SCREEN_WIDTH - textWidth - 10,
            10,
            textWidth,
            textHeight
        };
        
        SDL_RenderCopy(m_renderer->GetSDLRenderer(), scoreTexture, NULL, &textRect);
    }
}

void PlayingState::Cleanup() {
    std::cout << "PlayingState cleaned up\n";
}

void PlayingState::CheckGameOver() {
    if (!m_player->IsAlive()) {
        m_playState = PlayState::GAME_OVER;
        m_gameOverTimer.Start();
        m_scoreWasSaved = false;
        
        std::cout << "Game Over! Final Score: " << m_finalScore << std::endl;
        SaveHighScore();
    }
}

void PlayingState::SaveHighScore() {
    if (!m_scoreWasSaved && m_finalScore > 0) {
        m_saveSettings.SaveHighScore(m_finalScore);
        m_scoreWasSaved = true;
        std::cout << "Score saved: " << m_finalScore << std::endl;
    }
}

void PlayingState::RenderGameOver() {
    SDL_Renderer* sdlRenderer = m_renderer->GetSDLRenderer();
    
    SDL_Texture* gameOverTexture = m_assetManager->GetTextTexture(
        "game_over_text",
        "GAME OVER",
        {255, 0, 0, 255},
        "game_font",
        36
    );
    if (gameOverTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(gameOverTexture, NULL, NULL, &textWidth, &textHeight);
        
        SDL_Rect textRect = {
            (GameSettings::SCREEN_WIDTH - textWidth) / 2,
            (GameSettings::SCREEN_HEIGHT / 2) - 40,  
            textWidth,
            textHeight
        };
        SDL_RenderCopy(sdlRenderer, gameOverTexture, NULL, &textRect);
    }
    
    SDL_Texture* scoreTexture = m_assetManager->GetTextTexture(
        "final_score_text",
        "Score: " + std::to_string(m_finalScore),
        {255, 255, 255, 255},
        "game_font",
        28  
    );
    
    if (scoreTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(scoreTexture, NULL, NULL, &textWidth, &textHeight);
        
        SDL_Rect textRect = {
            (GameSettings::SCREEN_WIDTH - textWidth) / 2,
            (GameSettings::SCREEN_HEIGHT / 2),
            textWidth,
            textHeight
        };
        
        SDL_RenderCopy(sdlRenderer, scoreTexture, NULL, &textRect);
    }
    
    SDL_Texture* restartTexture = m_assetManager->GetTextTexture(
        "restart_text",
        "Press SPACE to restart or ESC to return to menu",
        {255, 255, 255, 255},
        "game_font",
        12
    );
    
    if (restartTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(restartTexture, NULL, NULL, &textWidth, &textHeight);
        
        SDL_Rect textRect = {
            (GameSettings::SCREEN_WIDTH - textWidth / 1.5) / 2,
            (GameSettings::SCREEN_HEIGHT / 2) + 50,  
            textWidth / 1.5,
            textHeight / 1.5
        };
        
        SDL_RenderCopy(sdlRenderer, restartTexture, NULL, &textRect);
    }
}

void PlayingState::Reset() {
    // Reset player
    if (m_player) {
        m_player->Reset();
        m_player->SetPosition(GameSettings::SCREEN_WIDTH / 2, GameSettings::SCREEN_HEIGHT - 100);
    }
    
    // Reset enemy spawner
    if (m_enemySpawner) {
        m_enemySpawner->Reset();
    }
    
    // Reset power-up spawner
    if (m_powerUpSpawner) {
        m_powerUpSpawner->Reset();
    }
    
    // Reset game state
    m_playState = PlayState::PLAYING;
    m_gameTimer.Start();
    m_finalScore = 0;
    m_currentTime = 0;
    m_scoreWasSaved = false;
}
