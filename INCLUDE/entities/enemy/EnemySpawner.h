#pragma once

#include <vector>
#include <memory>
#include <string>
#include <entities/enemy/Enemy.h>
#include <random>
#include <utils/Timer.h>
#include <utils/GameSettings.h>

class EnemySpawner {
public:
    static const int MAX_ENEMY_TYPES = 2;
    
    EnemySpawner(Renderer* renderer, AssetManager* assetManager);
    ~EnemySpawner();
    
    void Initialize();
    
    void Update(float deltaTime);
    
    void Render();
    
    bool CheckCollisions(Player* player);
    
    void Reset();
    
    float GetTimeSinceStart() const;
    
private:
    Renderer* m_renderer;
    AssetManager* m_assetManager;
    
    std::vector<std::shared_ptr<Enemy>> m_enemies;
    
    Enemy* m_enemyTemplates[MAX_ENEMY_TYPES];
    
    float m_spawnTimer;
    float m_spawnRate;
    
    Timer m_gameTimer;
    
    std::mt19937 m_rng;
    
    std::shared_ptr<Enemy> SpawnEnemy(int templateIndex);
    
    Vector2D GetRandomSpawnPosition();
};
