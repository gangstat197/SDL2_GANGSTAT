#pragma once

#include <entities/base/Entity.h>
#include <entities/base/Movement.h>
#include <functional>
#include <deque>

class Player;

enum class PowerUpType {
    INVINCIBLE,
    SIZE_REDUCTION,
    POINT_MULTIPLIER
};

class PowerUp : public Entity {
public:
    PowerUp(Renderer* renderer, AssetManager* assetManager, const std::string& textureId, PowerUpType type);
    ~PowerUp() override;
    
    void Update(float deltaTime) override;
    void Render() override;
    
    void SetSpeed(float speed);
    float GetSpeed() const;
    
    void SetRotationSpeed(float degreesPerSecond);
    float GetRotationSpeed() const;
    
    void ApplyEffect(Player* player);
    PowerUpType GetPowerUpType() const;
    
    bool CheckCollisionWithPlayer(Player* player);
    
protected:
    void CheckOffScreen();
    
private:
    PowerUpType m_powerUpType;
    
    // Trail effect properties
    std::deque<Vector2D> m_trailPositions;
    int m_maxTrailLength;
    float m_trailUpdateTime;
    float m_trailTimer;
    
    void UpdateTrail(float deltaTime);
    void RenderTrail();
}; 