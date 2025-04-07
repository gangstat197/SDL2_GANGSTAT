#include <entities/enemy/Enemy.h>
#include <entities/player/Player.h>
#include <cmath>
#include <iostream>

Enemy::Enemy(Renderer* renderer, AssetManager* assetManager, const std::string& textureId, 
            ColliderType colliderType)
    : Entity(renderer, assetManager, textureId, colliderType) {
    
    GetMovementComponent()->SetInitialPosition(GetPosition());
    GetRotationComponent()->SetRotationSpeed(0);
}

Enemy::~Enemy() {
}

void Enemy::Update(float deltaTime) {
    if (!IsActive()) return;
    
    Entity::Update(deltaTime);
    
    CheckOffScreen();
}

void Enemy::Render() {
    if (!IsActive()) return;
    
    Entity::Render();
}

void Enemy::SetSpeed(float speed) {
    GetMovementComponent()->SetSpeed(speed);
}

float Enemy::GetSpeed() const {
    return GetMovementComponent()->GetSpeed();
}

void Enemy::SetRotationSpeed(float degreesPerSecond) {
    GetRotationComponent()->SetRotationSpeed(degreesPerSecond);
}

float Enemy::GetRotationSpeed() const {
    return GetRotationComponent()->GetRotationSpeed();
}

void Enemy::SetMovementPattern(MovementPattern pattern) {
    GetMovementComponent()->SetPattern(pattern);
}

void Enemy::SetCustomMovementFunction(std::function<void(Entity*, float)> moveFunction) {
    GetMovementComponent()->SetCustomMovementFunction(moveFunction);
}

bool Enemy::CheckCollisionWithPlayer() {
    return GetColliderComponent()->CheckCollision();
}

void Enemy::CheckOffScreen() {
    GetMovementComponent()->CheckOffScreen(800, 850, 100);
}
