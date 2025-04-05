#pragma once

#include <entities/Entity.h>
#include <entities/CollisionHandler.h>
#include <functional>

// Forward declaration
class Player;

// Define movement pattern types
enum class MovementPattern {
    STRAIGHT,
    ZIGZAG,
    CURVE,
    SPIRAL,
    CUSTOM
};

// Define collider types
enum class ColliderType {
    RECTANGLE,  // Default from Entity
    CIRCLE,
    POLYGON
};

class Enemy : public Entity {
public:
    Enemy(Renderer* renderer, AssetManager* assetManager, const std::string& textureId);
    ~Enemy() override;
    
    void Update(float deltaTime) override;
    void Render() override;
    
    // Enemy-specific methods
    void SetSpeed(float speed);
    float GetSpeed() const;
    void SetDamage(int damage);
    int GetDamage() const;
    
    // ROTATION
    void SetRotationSpeed(float degreesPerSecond);
    float GetRotationSpeed() const;
    
    // MOVEMENT PATTERN
    void SetMovementPattern(MovementPattern pattern);
    void SetCustomMovementFunction(std::function<void(Enemy*, float)> moveFunction);
    
    // COLLISION
    bool CheckCollisionWithPlayer(Player* player) const;
    
    // Custom collider methods
    void SetColliderType(ColliderType type);
    ColliderType GetColliderType() const;
    
    // CIRCLE COLLIDER
    void SetCircleCollider(float radiusScale = 0.5f);
    CollisionCircle GetCircleCollider() const;
    
    // POLYGON COLLIDER
    void SetPolygonCollider(const std::vector<Vector2D>& points);
    void SetPolygonCollider(int sides, float radiusScale = 0.5f); // Regular polygon
    CollisionPolygon GetPolygonCollider() const;
    
    // DEBUG RENDERING
    void RenderCollider(bool showCollider);
    void RenderColliderDebug(); // Method for rendering debug information for colliders
    
protected:
    float m_speed;
    int m_damage;
    MovementPattern m_movementPattern;
    float m_rotationSpeed; // Degrees per second
    
    // TIME TRACKING
    float m_elapsedTime;
    
    // INITIAL POSITION
    Vector2D m_initialPosition;
    
    // CUSTOM MOVEMENT FUNCTION
    std::function<void(Enemy*, float)> m_customMoveFunction;
    
    // PATTERN-SPECIFIC MOVEMENT IMPLEMENTATIONS
    void MoveStraight(float deltaTime);
    void MoveZigZag(float deltaTime);
    void MoveCurve(float deltaTime);
    void MoveSpiral(float deltaTime);
    
    // HELPER METHODS
    void UpdateMovementPattern(float deltaTime);
    void CheckOffScreen();
    void UpdateCollider();
    
private:
    // CUSTOM COLLIDER PROPERTIES
    ColliderType m_colliderType;
    CollisionCircle m_circleCollider;
    CollisionPolygon m_polygonCollider;
    bool m_showCollider;
    
    // UPDATE CUSTOM COLLIDERS
    void UpdateCustomColliders();
};
