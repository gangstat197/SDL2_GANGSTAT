#include <entities/enemy/EnemySpawner.h>
#include <entities/player/Player.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>

EnemySpawner::EnemySpawner(Renderer* renderer, AssetManager* assetManager, int screenWidth, int screenHeight)
    : m_renderer(renderer),
      m_assetManager(assetManager),
      m_screenWidth(screenWidth),
      m_screenHeight(screenHeight),
      m_spawnTimer(0.0f),
      m_spawnRate(2.0f) {
    
    // Initialize random number generator
    std::random_device rd;
    m_rng = std::mt19937(rd());
    
    // Add default texture if none provided
    if (m_enemyTextureIds.empty()) {
        m_enemyTextureIds.push_back("enemy_default");
    }
}

EnemySpawner::~EnemySpawner() {
    // Clear all enemies to ensure proper cleanup
    m_enemies.clear();
}

void EnemySpawner::Update(float deltaTime, float difficulty) {
    // Update spawn timer and rate based on difficulty
    m_spawnTimer += deltaTime;
    m_spawnRate = std::max(0.5f, 2.0f - (difficulty * 0.1f)); // Decrease spawn rate as difficulty increases
    
    // Spawn new enemy when timer exceeds rate
    if (m_spawnTimer >= m_spawnRate) {
        m_spawnTimer = 0.0f;
        
        // Get random texture and pattern
        std::string textureId = GetRandomEnemyTextureId();
        MovementPattern pattern = GetRandomMovementPattern();
        
        // Spawn the enemy
        auto enemy = SpawnEnemy(textureId, pattern);
        
        // Customize based on difficulty
        enemy->SetSpeed(100.0f + (difficulty * 10.0f)); // Increase speed with difficulty
    }
    
    // Update all active enemies
    for (auto& enemy : m_enemies) {
        if (enemy && enemy->IsActive()) {
            enemy->Update(deltaTime);
        }
    }
    
    // Remove inactive enemies
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(), 
            [](const std::shared_ptr<Enemy>& enemy) {
                return !enemy || !enemy->IsActive();
            }),
        m_enemies.end()
    );
}

void EnemySpawner::Render() {
    for (auto& enemy : m_enemies) {
        if (enemy && enemy->IsActive()) {
            enemy->Render();
        }
    }
}

std::shared_ptr<Enemy> EnemySpawner::SpawnEnemy(const std::string& textureId, MovementPattern pattern) {
    // Create new enemy
    auto enemy = std::make_shared<Enemy>(m_renderer, m_assetManager, textureId, ColliderType::CIRCLE);
    
    // Set random spawn position
    Vector2D spawnPos = GetRandomSpawnPosition();
    enemy->SetPosition(spawnPos);
    
    // Set movement pattern
    enemy->SetMovementPattern(pattern);
    
    // Set a custom rotation speed based on the pattern type
    switch (pattern) {
        case MovementPattern::STRAIGHT:
            // Slow, consistent rotation for straight-moving enemies
            enemy->SetRotationSpeed(45.0f);
            break;
            
        case MovementPattern::ZIGZAG:
            // ZigZag looks good with faster rotation
            enemy->SetRotationSpeed(90.0f);
            break;
            
        case MovementPattern::CURVE:
            // Curved path with rotation in sync with the curve
            enemy->SetRotationSpeed(60.0f);
            break;
            
        case MovementPattern::SPIRAL:
            // Spiral enemies rotate with the spiral
            enemy->SetRotationSpeed(120.0f);
            break;
            
        case MovementPattern::CUSTOM:
        case MovementPattern::NONE:
        default:
            // Random rotation for custom patterns
            std::uniform_real_distribution<float> rotDist(-60.0f, 60.0f);
            enemy->SetRotationSpeed(rotDist(m_rng));
            break;
    }
    
    // Add to active enemies list
    m_enemies.push_back(enemy);
    
    return enemy;
}

bool EnemySpawner::CheckCollisions(Player* player) {
    // if (!player || !player->IsActive()) return false;
    
    // for (auto& enemy : m_enemies) {
    //     if (enemy && enemy->IsActive() && enemy->CheckCollisionWithPlayer(player)) {
    //         player->TakeDamage(0);
    //         return true;
    //     }
    // }
    
    // return false;
}

void EnemySpawner::Reset() {
    // Clear all enemies
    m_enemies.clear();
    
    // Reset spawn timer
    m_spawnTimer = 0.0f;
    m_spawnRate = 2.0f;
}

void EnemySpawner::AddEnemyTexture(const std::string& textureId) {
    if (std::find(m_enemyTextureIds.begin(), m_enemyTextureIds.end(), textureId) == m_enemyTextureIds.end()) {
        m_enemyTextureIds.push_back(textureId);
    }
}

Vector2D EnemySpawner::GetRandomSpawnPosition() {
    // Generate random x position across the screen width
    std::uniform_int_distribution<int> xDist(30, m_screenWidth - 30);
    
    // Spawn just above the top of the screen
    int x = xDist(m_rng);
    int y = -30; // Just above top edge
    
    return Vector2D(x, y);
}

MovementPattern EnemySpawner::GetRandomMovementPattern() {
    // Generate random pattern
    std::uniform_int_distribution<int> patternDist(0, 3); // 4 patterns (0-3)
    int pattern = patternDist(m_rng);
    
    switch (pattern) {
        case 0: return MovementPattern::STRAIGHT;
        case 1: return MovementPattern::ZIGZAG;
        case 2: return MovementPattern::CURVE;
        case 3: return MovementPattern::SPIRAL;
        default: return MovementPattern::STRAIGHT;
    }
}

std::string EnemySpawner::GetRandomEnemyTextureId() {
    if (m_enemyTextureIds.empty()) {
        return "enemy_default";
    }
    
    std::uniform_int_distribution<int> textureDist(0, m_enemyTextureIds.size() - 1);
    int index = textureDist(m_rng);
    
    return m_enemyTextureIds[index];
}
