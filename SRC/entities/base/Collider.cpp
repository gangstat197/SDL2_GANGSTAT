#include <entities/base/Collider.h>
#include <entities/base/Entity.h>
#include <SDL2/SDL.h> 
#include <cmath>
#include <iostream>

Collider::Collider(Entity* owner, ColliderType colliderType)
    : m_owner(owner),
      m_colliderType(colliderType),
      m_radius(0.0f) {
    // Initialize empty points
    m_points.clear();
    m_originalPoints.clear();
}

Collider::~Collider() {
}

void Collider::Update(float deltaTime) {
  if (!m_owner) return;
  // std::cout << "Updating collider" << std::endl;
  
  UpdateMovement();

  // std::cout << "Updating collider" << std::endl;
  // std::cout << "m_points: " << m_points.size() << std::endl;
  UpdateRotation();
}

void Collider::UpdateMovement() {
  SetPosition(m_owner->GetPosition());
}

void Collider::UpdateRotation() {
  if (m_colliderType == ColliderType::CIRCLE) {
    return;
  }
  
  if (m_originalPoints.empty()) {
    return; // Don't try to rotate if we have no points
  }
  
  float rotation = m_owner->GetRotation();
  Vector2D center = m_owner->GetPosition();
  float angle = rotation * M_PI / 180.0f; 

  m_points.clear();


  for (auto& point : m_originalPoints) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    
    float newX = point.x * cosAngle - point.y * sinAngle;
    float newY = point.x * sinAngle + point.y * cosAngle;
    
    m_points.push_back(center + Vector2D(newX, newY));
  }
}

ColliderType Collider::GetColliderType() const {
    return m_colliderType;
}

std::vector<Vector2D> Collider::GetPoints() const {
    return m_points;
}

float Collider::GetRadius() const {
    return m_radius;
}

void Collider::SetPosition(const Vector2D& position) {
  if (m_colliderType == ColliderType::POLYGON) {
    if (!m_owner) return;
    
    if (!m_points.empty()) {
      UpdateRotation();
    }
  }
}

Vector2D Collider::GetPosition() const {
  return m_owner->GetPosition();
}

void Collider::SetColliderType(ColliderType type) {
    m_colliderType = type;
}

void Collider::SetPolygonCollider(int numPoints, std::vector<Vector2D>* points) {
    SetColliderType(ColliderType::POLYGON);
    if (points && !points->empty()) {
        m_originalPoints = *points;
        m_points = *points;
        
        if (m_owner) {
            UpdateRotation();
        }
    }
}

void Collider::SetCircleCollider(float radius) {
    SetColliderType(ColliderType::CIRCLE);
    m_radius = radius;
}

void Collider::RenderColliderDebug() {
    if (!m_owner) return;
    
    SDL_Renderer* renderer = m_owner->GetRenderer()->GetSDLRenderer();
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    
    if (m_colliderType == ColliderType::CIRCLE) {
        const int segments = 32;
        for (int i = 0; i < segments; i++) {
            float angle1 = (float)i / segments * 2 * M_PI;
            float angle2 = (float)(i + 1) / segments * 2 * M_PI;
            
            int x1 = static_cast<int>(m_owner->GetPosition().x + cos(angle1) * m_radius);
            int y1 = static_cast<int>(m_owner->GetPosition().y + sin(angle1) * m_radius);
            int x2 = static_cast<int>(m_owner->GetPosition().x + cos(angle2) * m_radius);
            int y2 = static_cast<int>(m_owner->GetPosition().y + sin(angle2) * m_radius);
            
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    } else if (m_colliderType == ColliderType::POLYGON) {
        Vector2D center = m_owner->GetPosition();
        
        for (size_t i = 0; i < m_points.size(); i++) {
            size_t next = (i + 1) % m_points.size();
            
            int x1 = static_cast<int>(m_points[i].x);
            int y1 = static_cast<int>(m_points[i].y);
            int x2 = static_cast<int>(m_points[next].x);
            int y2 = static_cast<int>(m_points[next].y);
            
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }
}

bool Collider::CheckCollision(Collider* playerCollider) {
  // CHECK IF THE POLYGON IS COLLIDING WITH MOUSE CURSOR 
  if (m_colliderType == ColliderType::POLYGON) {
    return CheckPolygonCollision(playerCollider);
  }

  if (m_colliderType == ColliderType::CIRCLE) {
    return CheckCircleCollision(playerCollider);
  }

  return false;
}

bool Collider::CheckPolygonCollision(Collider* playerCollider) {
  std::vector<Vector2D> playerPoints = playerCollider->GetPoints();

  std::vector<Vector2D> edges;

  for (int i = 0; i < m_points.size(); i++) {
    int next = (i + 1) % m_points.size();
    Vector2D A = m_points[i];
    Vector2D B = m_points[next];

    Vector2D edge = B - A;
    Vector2D normal = edge.perpendicular().normalize();

    edges.push_back(normal);
  }

  for (int i = 0; i < playerPoints.size(); i++) {
    int next = (i + 1) % playerPoints.size();
    Vector2D A = playerPoints[i];
    Vector2D B = playerPoints[next];
    
    Vector2D edge = B - A;
    Vector2D normal = edge.perpendicular().normalize();

    edges.push_back(normal);
  }

  for (int i = 0; i < edges.size(); i++) {
    Vector2D edge = edges[i];
    float minA = std::numeric_limits<float>::infinity();
    float maxA = -std::numeric_limits<float>::infinity();
    
    float minB = std::numeric_limits<float>::infinity();
    float maxB = -std::numeric_limits<float>::infinity();

    for (int j = 0; j < m_points.size(); j++) {
      float projection = m_points[j] * edge;
      minA = std::min(minA, projection);
      maxA = std::max(maxA, projection);
    }

    for (int j = 0; j < playerPoints.size(); j++) {
      float projection = playerPoints[j] * edge;
      minB = std::min(minB, projection);
      maxB = std::max(maxB, projection);
    }

    if (maxA < minB || maxB < minA) { 
      return false;
    }
  }

  return true;
}

bool Collider::CheckCircleCollision(Collider* playerCollider) {
  std::vector<Vector2D> playerPoints = playerCollider->GetPoints();

  for (int i = 0; i < playerPoints.size(); i++) {
    float distance = (m_owner->GetPosition() - playerPoints[i]).magnitude();
    if (distance <= m_radius) {
      return true;
    }
  }

  return false;
}
