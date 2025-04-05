#pragma once

#include <vector>
#include <memory>
#include <string>
#include <entities/Enemy.h>
#include <random>

class EnemySpawner {
public:
    EnemySpawner(Renderer* renderer, AssetManager* assetManager, int screenWidth, int screenHeight);
    ~EnemySpawner();
    
    // Update all enemies and handle spawning
    void Update(float deltaTime, float difficulty);
    
    // Render all active enemies
    void Render();
    
    // Spawn a specific enemy type
    std::shared_ptr<Enemy> SpawnEnemy(const std::string& textureId, MovementPattern pattern);
    
    // Check collisions with player
    bool CheckCollisions(Player* player);
    
    // Reset the spawner (e.g., for new game)
    void Reset();
    
    // Add enemy texture to be used for spawning (allows for variety)
    void AddEnemyTexture(const std::string& textureId);
    
private:
    Renderer* m_renderer;
    AssetManager* m_assetManager;
    int m_screenWidth;
    int m_screenHeight;
    
    // List of active enemies
    std::vector<std::shared_ptr<Enemy>> m_enemies;
    
    // List of enemy textures to use
    std::vector<std::string> m_enemyTextureIds;
    
    // Spawn timers and rates
    float m_spawnTimer;
    float m_spawnRate;
    
    // Random number generation
    std::mt19937 m_rng;
    
    // Helper methods for spawning
    Vector2D GetRandomSpawnPosition();
    MovementPattern GetRandomMovementPattern();
    std::string GetRandomEnemyTextureId();
};
