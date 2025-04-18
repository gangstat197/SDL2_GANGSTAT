#pragma once

#include <entities/base/Entity.h>
#include <entities/base/Movement.h>
#include <functional>

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
};
