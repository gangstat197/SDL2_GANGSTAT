#pragma once

// Forward declarations
class Entity;

class Rotation {
public:
    Rotation(Entity* owner);
    ~Rotation();
    
    // Update the entity's rotation based on rotation speed
    void Update(float deltaTime);
    
    // Set rotation properties
    void SetRotationSpeed(float degreesPerSecond);
    float GetRotationSpeed() const;
    
    // Normalize rotation to be within 0-360 degrees
    void NormalizeRotation();
    
private:
    Entity* m_owner;
    float m_rotationSpeed; // Degrees per second
};
