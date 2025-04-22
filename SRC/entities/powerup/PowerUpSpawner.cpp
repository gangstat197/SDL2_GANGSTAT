#include <entities/powerup/PowerUpSpawner.h>
#include <entities/player/Player.h>
#include <utils/GameSettings.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>

PowerUpSpawner::PowerUpSpawner(Renderer* renderer, AssetManager* assetManager)
    : m_renderer(renderer),
      m_assetManager(assetManager),
      m_spawnTimer(0.0f),
      m_spawnRate(2.0f) { // Reduced from 10.0f to 5.0f for more frequent checks
    
    std::random_device rd;
    m_rng = std::mt19937(rd());
    
    for (int i = 0; i < POWERUP_TYPES; i++) {
        m_powerUpTemplates[i] = nullptr;
    }
    
    m_gameTimer.Start();
}

PowerUpSpawner::~PowerUpSpawner() {
    Reset();
    
    for (int i = 0; i < POWERUP_TYPES; i++) {
        if (m_powerUpTemplates[i]) {
            delete m_powerUpTemplates[i];
            m_powerUpTemplates[i] = nullptr;
        }
    }
}

void PowerUpSpawner::Initialize() {
    // Invincibility power-up
    m_powerUpTemplates[0] = new PowerUp(m_renderer, m_assetManager, "powerup_invincible", PowerUpType::INVINCIBLE);
    m_powerUpTemplates[0]->SetSpeed(GameSettings::ENEMY_BASE_SPEED); // Slower than enemies
    m_powerUpTemplates[0]->SetRotationSpeed(45.0f);
    
    // Size reduction power-up
    m_powerUpTemplates[1] = new PowerUp(m_renderer, m_assetManager, "powerup_ant", PowerUpType::SIZE_REDUCTION);
    m_powerUpTemplates[1]->SetSpeed(GameSettings::ENEMY_BASE_SPEED);
    m_powerUpTemplates[1]->SetRotationSpeed(30.0f);
    
    // Point multiplier power-up
    m_powerUpTemplates[2] = new PowerUp(m_renderer, m_assetManager, "powerup_midas", PowerUpType::POINT_MULTIPLIER);
    m_powerUpTemplates[2]->SetSpeed(GameSettings::ENEMY_BASE_SPEED);
    m_powerUpTemplates[2]->SetRotationSpeed(60.0f);
}

void PowerUpSpawner::Update(float deltaTime) {
    m_spawnTimer += deltaTime;
    
    float timeSinceStart = m_gameTimer.GetSeconds();
    
    float currentSpawnChance = GameSettings::POWERUP_SPAWN_CHANCE;
    if (timeSinceStart > 30.0f) {
        currentSpawnChance = GameSettings::POWERUP_SPAWN_CHANCE * 1.5f;
    }
    if (timeSinceStart > 60.0f) {
        currentSpawnChance = GameSettings::POWERUP_SPAWN_CHANCE * 2.0f;
    }
    
    if (m_spawnTimer >= m_spawnRate) {
        m_spawnTimer = 0.0f;
        
        std::uniform_real_distribution<float> spawnChanceDist(0.0f, 1.0f);
        if (spawnChanceDist(m_rng) <= currentSpawnChance) {
            std::uniform_int_distribution<int> typeDist(0, POWERUP_TYPES - 1);
            int templateIndex = typeDist(m_rng);
            
            SpawnPowerUp(templateIndex);
            
            m_spawnRate = 4.0f + (4.0f * spawnChanceDist(m_rng));
        } else {
            m_spawnRate = 2.0f + (3.0f * spawnChanceDist(m_rng));
        }
    }
    
    for (auto& powerUp : m_powerUps) {
        if (powerUp && powerUp->IsActive()) {
            powerUp->Update(deltaTime);
        }
    }
    
    m_powerUps.erase(
        std::remove_if(m_powerUps.begin(), m_powerUps.end(), 
            [](const std::shared_ptr<PowerUp>& powerUp) {
                return !powerUp || !powerUp->IsActive();
            }),
        m_powerUps.end()
    );
}

void PowerUpSpawner::Render() {
    for (auto& powerUp : m_powerUps) {
        if (powerUp && powerUp->IsActive()) {
            powerUp->Render();
        }
    }
}

std::shared_ptr<PowerUp> PowerUpSpawner::SpawnPowerUp(int templateIndex) {
    if (templateIndex < 0 || templateIndex >= POWERUP_TYPES || !m_powerUpTemplates[templateIndex]) {
        return nullptr;
    }
    
    PowerUp* templatePowerUp = m_powerUpTemplates[templateIndex];
    
    auto powerUp = std::make_shared<PowerUp>(
        m_renderer,
        m_assetManager,
        templatePowerUp->GetTextureId(),
        templatePowerUp->GetPowerUpType()
    );
    
    // Set the position and movement properties
    Vector2D spawnPos = GetRandomSpawnPosition();
    powerUp->SetInitialPosition(spawnPos);
    powerUp->SetSpeed(templatePowerUp->GetSpeed());
    powerUp->SetRotationSpeed(templatePowerUp->GetRotationSpeed());
    
    // Add to the active power-ups list
    m_powerUps.push_back(powerUp);
    
    return powerUp;
}

bool PowerUpSpawner::CheckCollisions(Player* player) {
    if (!player || !player->IsActive()) return false;
    
    bool collisionOccurred = false;
    
    for (auto& powerUp : m_powerUps) {
        if (powerUp && powerUp->IsActive() && powerUp->CheckCollisionWithPlayer(player)) {
            powerUp->ApplyEffect(player);
            collisionOccurred = true;
        }
    }
    
    return collisionOccurred;
}

void PowerUpSpawner::Reset() {
    m_powerUps.clear();
    m_spawnTimer = 0.0f;
    m_spawnRate = 5.0f;
    m_gameTimer.Start();
}

Vector2D PowerUpSpawner::GetRandomSpawnPosition() {
    int marginX = GameSettings::SCREEN_WIDTH * 0.05;
    int minX = marginX;
    int maxX = GameSettings::SCREEN_WIDTH - marginX;
    
    std::uniform_int_distribution<int> xDist(minX, maxX);
    int x = xDist(m_rng);
    
    int y = -30; 
    
    return Vector2D(x, y);
} 