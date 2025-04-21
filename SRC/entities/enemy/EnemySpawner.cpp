#include <entities/enemy/EnemySpawner.h>
#include <entities/player/Player.h>
#include <utils/GameSettings.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>

EnemySpawner::EnemySpawner(Renderer* renderer, AssetManager* assetManager)
    : m_renderer(renderer),
      m_assetManager(assetManager),
      m_spawnTimer(0.0f),
      m_spawnRate(GameSettings::SPAWN_RATE_MS / 1000.0f) {
    
    std::random_device rd;
    m_rng = std::mt19937(rd());
    
    for (int i = 0; i < MAX_ENEMY_TYPES; i++) {
        m_enemyTemplates[i] = nullptr;
    }
    
    m_gameTimer.Start();
}

EnemySpawner::~EnemySpawner() {
    Reset();
    
    for (int i = 0; i < MAX_ENEMY_TYPES; i++) {
        if (m_enemyTemplates[i]) {
            delete m_enemyTemplates[i];
            m_enemyTemplates[i] = nullptr;
        }
    }
}

void EnemySpawner::Initialize() {    
    Vector2D colliderPoints[] = {
        Vector2D(-10, -10),
        Vector2D(10, -10),
        Vector2D(10, 10),
        Vector2D(-10, 10)
    };
    

    m_enemyTemplates[0] = new Enemy(m_renderer, m_assetManager, "enemy_round", ColliderType::CIRCLE);
    m_enemyTemplates[0]->SetMovementPattern(MovementPattern::STRAIGHT);
    m_enemyTemplates[0]->SetSpeed(GameSettings::ENEMY_BASE_SPEED);
    m_enemyTemplates[0]->SetRotationSpeed(45.0f);
    int width = m_enemyTemplates[0]->GetWidth();
    m_enemyTemplates[0]->GetColliderComponent()->SetCircleCollider(width/2);

    m_enemyTemplates[1] = new Enemy(m_renderer, m_assetManager, "enemy_round01", ColliderType::CIRCLE);
    m_enemyTemplates[1]->SetMovementPattern(MovementPattern::ZIGZAG);
    m_enemyTemplates[1]->SetSpeed(GameSettings::ENEMY_BASE_SPEED);
    m_enemyTemplates[1]->SetRotationSpeed(90.0f);
    width = m_enemyTemplates[1]->GetWidth();
    m_enemyTemplates[1]->GetColliderComponent()->SetCircleCollider(width/2);
}

void EnemySpawner::Update(float deltaTime) {
    m_spawnTimer += deltaTime;
    
    float timeSinceStart = GetTimeSinceStart();
    float difficulty = 1.0f + (timeSinceStart / (GameSettings::DIFFICULTY_INCREASE_TIME_MS / 1000.0f)); 

    m_spawnRate = std::max(0.5f, (GameSettings::SPAWN_RATE_MS / 1000.0f) - (difficulty * 0.1f));
    
    if (m_spawnTimer >= m_spawnRate) {
        m_spawnTimer = 0.0f;
        
        std::uniform_int_distribution<int> dist(0, MAX_ENEMY_TYPES - 1);
        int templateIndex = dist(m_rng);
        
        auto enemy = SpawnEnemy(templateIndex);
        if (enemy) {
            float speedMultiplier = 1.0f + (difficulty * 0.1f);
            enemy->SetSpeed(enemy->GetSpeed() * speedMultiplier);
        }
    }
    
    for (auto& enemy : m_enemies) {
        if (enemy && enemy->IsActive()) {
            enemy->Update(deltaTime);
        }
    }
    
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

std::shared_ptr<Enemy> EnemySpawner::SpawnEnemy(int templateIndex) {
    if (templateIndex < 0 || templateIndex >= MAX_ENEMY_TYPES || !m_enemyTemplates[templateIndex]) {
        return nullptr;
    }
    
    Enemy* templateEnemy = m_enemyTemplates[templateIndex];
    ColliderType colliderType = templateEnemy->GetColliderType();
    
    auto enemy = std::make_shared<Enemy>(m_renderer, m_assetManager, 
                                       templateEnemy->GetTextureId(), 
                                       colliderType);
    
    Vector2D spawnPos = GetRandomSpawnPosition();
    enemy->SetInitialPosition(spawnPos);
    
    enemy->SetMovementPattern(templateEnemy->GetMovementComponent()->GetPattern());
    enemy->SetSpeed(templateEnemy->GetMovementComponent()->GetSpeed());
    enemy->SetRotationSpeed(templateEnemy->GetRotationComponent()->GetRotationSpeed());
    
    switch (colliderType) {
        case ColliderType::CIRCLE: {
            int width = enemy->GetWidth();
            enemy->GetColliderComponent()->SetCircleCollider(width/2);
            break;
        }
        case ColliderType::POLYGON: {
            Collider* templateCollider = templateEnemy->GetColliderComponent();
            if (templateCollider) {
                const std::vector<Vector2D>& points = templateCollider->GetPolygonPoints();
                if (!points.empty()) {
                    enemy->GetColliderComponent()->SetPolygonCollider(points.size(), points);
                }
            }
            break;
        }
    }
    
    m_enemies.push_back(enemy);
    
    return enemy;
}

bool EnemySpawner::CheckCollisions(Player* player) {
    if (!player || !player->IsActive()) return false;
    
    for (auto& enemy : m_enemies) {
        if (enemy && enemy->IsActive() && enemy->CheckCollisionWithPlayer(player)) {
            if (!player->IsInvincible()) {
                player->Hit();
                return true;
            }
        }
    }
    
    return false;
}

void EnemySpawner::Reset() {
    m_enemies.clear();
    
    m_spawnTimer = 0.0f;
    m_spawnRate = GameSettings::SPAWN_RATE_MS / 1000.0f;
    m_gameTimer.Start();
}

float EnemySpawner::GetTimeSinceStart() const {
    return m_gameTimer.GetSeconds();
}

Vector2D EnemySpawner::GetRandomSpawnPosition() {
    int marginX = GameSettings::SCREEN_WIDTH * 0.05;
    int minX = marginX;
    int maxX = GameSettings::SCREEN_WIDTH - marginX;
    
    std::uniform_int_distribution<int> xDist(minX, maxX);
    int x = xDist(m_rng);
    
    int y = -30;
    
    return Vector2D(x, y);
}

