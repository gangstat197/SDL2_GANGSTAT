#pragma once

#include <entities/base/Entity.h>
#include <entities/base/Movement.h>
#include <functional>
#include <deque>

class Player;

class Enemy : public Entity {
public:
    Enemy(Renderer* renderer, AssetManager* assetManager, const std::string& textureId, ColliderType colliderType);
    ~Enemy() override;
    
    void Update(float deltaTime) override;
    void Render() override;
    
    void SetSpeed(float speed);
    float GetSpeed() const;
    
    void SetRotationSpeed(float degreesPerSecond);
    float GetRotationSpeed() const;
    
    void SetMovementPattern(MovementPattern pattern);
    
    bool CheckCollisionWithPlayer(Player* player);
    
protected:
    // Helper methods
    void CheckOffScreen();
    
private:
    // Trail effect properties
    std::deque<Vector2D> m_trailPositions;
    int m_maxTrailLength;
    float m_trailUpdateTime;
    float m_trailTimer;
    
    void UpdateTrail(float deltaTime);
    void RenderTrail();
};
