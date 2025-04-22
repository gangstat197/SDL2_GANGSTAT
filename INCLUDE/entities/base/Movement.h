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
    
    void Update(float deltaTime);
    
    void SetSpeed(float speed);
    float GetSpeed() const;
    
    void SetPattern(MovementPattern pattern);
    MovementPattern GetPattern() const;
    
    void SetInitialPosition(const Vector2D& position);
    
    void CheckOffScreen(int screenWidth = 800, int screenHeight = 600, int margin = 100);

    void SetZigzagConfig(float amplitudeRatio, float freqRatio);
    float GetAmplitudeRatio() const { return m_amplitudeRatio; }
    float GetFrequencyRatio() const { return m_freqRatio; }
    
private:
    Entity* m_owner;
    float m_speed;
    MovementPattern m_pattern;
    float m_elapsedTime;
    Vector2D m_initialPosition;

    float m_amplitudeRatio;
    float m_freqRatio;
    
    // Movement pattern implementations
    void MoveStraight(float deltaTime);
    void MoveZigZag(float deltaTime);
    void MoveCurve(float deltaTime);
    void MoveSpiral(float deltaTime);
};
