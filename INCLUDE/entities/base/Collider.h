#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <utils/Vector2D.h>

// Forward declarations
class Entity;

enum class ColliderType {
    CIRCLE,     
    POLYGON     
};

class Collider {
public:
    Collider(Entity* owner, ColliderType colliderType);
    ~Collider();
    
    void Update(float deltaTime = 0.0f);
    
    void UpdateMovement();
    void UpdateRotation();
    
    ColliderType GetColliderType() const;
    std::vector<Vector2D> GetPoints() const;
    float GetRadius() const;
    Vector2D GetPosition() const;
    
    void SetPosition(const Vector2D& position);
    void SetColliderType(ColliderType type);
    
    void SetPolygonCollider(int numPoints, std::vector<Vector2D>* points);
    void SetCircleCollider(float radius);
    
    void RenderColliderDebug();
    
    bool CheckCollision(Collider* playerCollider);
    bool CheckPolygonCollision(Collider* playerCollider);
    bool CheckCircleCollision(Collider* playerCollider);

private:
    Entity* m_owner;
    ColliderType m_colliderType;
    float m_radius;
    std::vector<Vector2D> m_points;
    std::vector<Vector2D> m_originalPoints; // Original polygon points before rotation
};


