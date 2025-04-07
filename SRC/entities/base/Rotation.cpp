#include <entities/base/Rotation.h>
#include <entities/base/Entity.h>

Rotation::Rotation(Entity* owner)
    : m_owner(owner),
      m_rotationSpeed(0.0f) {
}

Rotation::~Rotation() {
}

void Rotation::Update(float deltaTime) {
    if (!m_owner || !m_owner->IsActive() || m_rotationSpeed == 0.0f) return;
    
    float rotation = m_owner->GetRotation() + m_rotationSpeed * deltaTime;
    
    while (rotation >= 360.0f) {
        rotation -= 360.0f;
    }
    while (rotation < 0.0f) {
        rotation += 360.0f;
    }

    m_owner->SetRotation(rotation);
}

void Rotation::SetRotationSpeed(float degreesPerSecond) {
    m_rotationSpeed = degreesPerSecond;
}

float Rotation::GetRotationSpeed() const {
    return m_rotationSpeed;
}

void Rotation::NormalizeRotation() {
    if (!m_owner) return;
    
    float rotation = m_owner->GetRotation();
    
    while (rotation >= 360.0f) {
        rotation -= 360.0f;
    }
    while (rotation < 0.0f) {
        rotation += 360.0f;
    }
    
    m_owner->SetRotation(rotation);
}
