#pragma once

#include <vector>
#include <memory>
#include <string>
#include <entities/powerup/PowerUp.h>
#include <random>
#include <utils/Timer.h>
#include <utils/GameSettings.h>

class PowerUpSpawner {
public:
    static const int POWERUP_TYPES = 3;
    
    PowerUpSpawner(Renderer* renderer, AssetManager* assetManager);
    ~PowerUpSpawner();
    
    void Initialize();
    void Update(float deltaTime);
    void Render();
    
    bool CheckCollisions(Player* player);
    
    void Reset();
    
private:
    Renderer* m_renderer;
    AssetManager* m_assetManager;
    
    std::vector<std::shared_ptr<PowerUp>> m_powerUps;
    
    PowerUp* m_powerUpTemplates[POWERUP_TYPES];
    
    float m_spawnTimer;
    float m_spawnRate;
    
    Timer m_gameTimer;
    std::mt19937 m_rng;
    
    std::shared_ptr<PowerUp> SpawnPowerUp(int templateIndex);
    Vector2D GetRandomSpawnPosition();
}; 