#pragma once

#include <utils/Vector2D.h>
#include <functional>

// Forward declarations
class Entity;

enum class MovementPattern {
    NONE,
    STRAIGHT,
    ZIGZAG,
    CURVE,
    SPIRAL
};

class Movement {
public:
    Movement(Entity* owner);
    ~Movement();
    
    // Update the entity's position based on current movement pattern
    void Update(float deltaTime);
    
    // Set movement properties
    void SetSpeed(float speed);
    float GetSpeed() const;
    
    // Set and get pattern
    void SetPattern(MovementPattern pattern);
    MovementPattern GetPattern() const;
    
    // Set initial position (reference point for patterns)
    void SetInitialPosition(const Vector2D& position);
    
    // Utility function to check if entity is off screen and deactivate it
    void CheckOffScreen(int screenWidth = 800, int screenHeight = 600, int margin = 100);
    
private:
    Entity* m_owner;
    float m_speed;
    MovementPattern m_pattern;
    float m_elapsedTime;
    Vector2D m_initialPosition;
    
    // Movement pattern implementations
    void MoveStraight(float deltaTime);
    void MoveZigZag(float deltaTime);
    void MoveCurve(float deltaTime);
    void MoveSpiral(float deltaTime);
};
