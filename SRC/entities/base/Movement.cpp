#include <entities/base/Movement.h>
#include <entities/base/Entity.h>
#include <cmath>

Movement::Movement(Entity* owner)
    : m_owner(owner),
      m_speed(100.0f),
      m_pattern(MovementPattern::NONE),
      m_elapsedTime(0.0f) {
    m_initialPosition = owner ? owner->GetPosition() : Vector2D(0, 0);
}

Movement::~Movement() {
}

void Movement::Update(float deltaTime) {
    if (!m_owner || !m_owner->IsActive()) return;
    
    m_elapsedTime += deltaTime;
    
    switch (m_pattern) {
        case MovementPattern::STRAIGHT:
            MoveStraight(deltaTime);
            break;
        case MovementPattern::ZIGZAG:
            MoveZigZag(deltaTime);
            break;
        case MovementPattern::CURVE:
            MoveCurve(deltaTime);
            break;
        case MovementPattern::SPIRAL:
            MoveSpiral(deltaTime);
            break;
        case MovementPattern::NONE:
        default:
            break;
    }
}

void Movement::SetSpeed(float speed) {
    m_speed = speed;
}

float Movement::GetSpeed() const {
    return m_speed;
}

void Movement::SetPattern(MovementPattern pattern) {
    m_pattern = pattern;
    m_elapsedTime = 0.0f; 
}

MovementPattern Movement::GetPattern() const {
    return m_pattern;
}

void Movement::SetInitialPosition(const Vector2D& position) {
    m_initialPosition = position;
}

void Movement::CheckOffScreen(int screenWidth, int screenHeight, int margin) {
    if (!m_owner) return;
    
    Vector2D pos = m_owner->GetPosition();
    
    if (pos.y > screenHeight + margin) {
        m_owner->SetActive(false);
    }
    
    if (pos.x < -margin || pos.x > screenWidth + margin) {
        m_owner->SetActive(false);
    }
}

void Movement::MoveStraight(float deltaTime) {
    if (!m_owner) return;
    
    Vector2D pos = m_owner->GetPosition();
    pos.y += m_speed * deltaTime;
    m_owner->SetPosition(pos);
}

void Movement::MoveZigZag(float deltaTime) {
    if (!m_owner) return;
    
    Vector2D pos = m_owner->GetPosition();
    pos.y += m_speed * deltaTime;
    
    float amplitude = 50.0f; 
    float frequency = 2.0f;
    
    pos.x = m_initialPosition.x + amplitude * sin(frequency * m_elapsedTime);
    
    m_owner->SetPosition(pos);
}

void Movement::MoveCurve(float deltaTime) {
    if (!m_owner) return;
    
    Vector2D pos = m_owner->GetPosition();
    
    float a = 0.01f; 
    float verticalSpeed = m_speed * 0.5f * deltaTime; 
    
    float distanceFromStart = pos.y - m_initialPosition.y;
    pos.x = m_initialPosition.x + a * distanceFromStart * distanceFromStart;
    
    pos.y += verticalSpeed;
    
    m_owner->SetPosition(pos);
}

void Movement::MoveSpiral(float deltaTime) {
    if (!m_owner) return;
    
    float radius = 10.0f * m_elapsedTime;
    float angle = 5.0f * m_elapsedTime;
    
    Vector2D pos;
    pos.x = m_initialPosition.x + radius * cos(angle);
    pos.y = m_initialPosition.y + radius * sin(angle) + m_speed * deltaTime;
    
    m_owner->SetPosition(pos);
}
