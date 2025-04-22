#include <entities/base/Entity.h>
#include <iostream>

Entity::Entity(Renderer* renderer, AssetManager* assetManager, const std::string& textureId, ColliderType colliderType)
    : m_renderer(renderer),
      m_assetManager(assetManager),
      m_textureId(textureId),
      m_position(0, 0),
      m_scale(1.0f),
      m_rotation(0.0f),
      m_width(0),
      m_height(0),
      m_isActive(true) {
    
    std::cout << "Entity created\n";
    SDL_Texture* texture = m_assetManager->GetTexture(textureId);
    if (texture) {
        SDL_QueryTexture(texture, nullptr, nullptr, &m_width, &m_height);
    }
    
    std::cout << "Creating Entity's components\n";
    m_movement = std::make_unique<Movement>(this);
    m_rotationComponent = std::make_unique<Rotation>(this);
    m_collider = std::make_unique<Collider>(this, colliderType);
}

Entity::~Entity() {}

void Entity::Update(float deltaTime) {
    if (!m_isActive) return;
    
    if (m_movement) {
        m_movement->Update(deltaTime);
    }
    
    if (m_rotationComponent) {
        m_rotationComponent->Update(deltaTime);
    }
    
    UpdateCollider();
}

void Entity::Render() {
    if (!m_isActive) return;
    
    SDL_Texture* texture = m_assetManager->GetTexture(m_textureId);
    int scaledWidth = static_cast<int>(m_width * m_scale);
    int scaledHeight = static_cast<int>(m_height * m_scale);
    m_renderer->RenderTexture(texture, m_position.x, m_position.y, scaledWidth, scaledHeight, m_rotation);

    RenderDebug();
}

void Entity::RenderDebug() {
    m_collider->RenderColliderDebug();
    
    m_renderer->RenderTexture(m_assetManager->GetTexture("debug_position"), m_position.x, m_position.y, 10, 10, 0);
}

void Entity::SetPosition(const Vector2D& position) {
    m_position = position;
    UpdateCollider();
}

void Entity::SetInitialPosition(const Vector2D& position) {
    m_movement->SetInitialPosition(position);
    SetPosition(position);
}


void Entity::SetPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
    UpdateCollider();
}

Vector2D Entity::GetPosition() const {
    return m_position;
}

void Entity::SetScale(float scale) {
    m_scale = scale;
    if (m_collider) {
        if (m_collider->GetColliderType() == ColliderType::CIRCLE) {
            float scaledRadius = (m_width * m_scale) / 2.0f;
            m_collider->SetCircleCollider(scaledRadius);
        } else if (m_collider->GetColliderType() == ColliderType::POLYGON) {
            std::vector<Vector2D> originalPoints = m_collider->GetPolygonPoints();
            std::vector<Vector2D> scaledPoints;
            
            float scaleRatio = m_scale;
            for (const auto& point : originalPoints) {
                Vector2D scaledPoint = point * scaleRatio;
                scaledPoints.push_back(scaledPoint);
            }
            
            m_collider->SetPolygonCollider(scaledPoints.size(), scaledPoints);
        }
    }
    UpdateCollider();
}

float Entity::GetScale() const {
    return m_scale;
}

void Entity::SetSize(int width, int height) {
    m_width = width;
    m_height = height;
    UpdateCollider();
}

int Entity::GetWidth() const {
    return m_width;
}

int Entity::GetHeight() const {
    return m_height;
}


void Entity::SetActive(bool active) {
    m_isActive = active;
}

bool Entity::IsActive() const {
    return m_isActive;
}

Movement* Entity::GetMovementComponent() const {
    return m_movement.get();
}

Rotation* Entity::GetRotationComponent() const {
    return m_rotationComponent.get();
}

Collider* Entity::GetColliderComponent() const {
    return m_collider.get();
}

ColliderType Entity::GetColliderType() const {
    if (m_collider) {
        return m_collider->GetColliderType();
    }
    return ColliderType::CIRCLE;
}

void Entity::SetNewCollider(Collider* newCollider) {
    if (newCollider) {
        m_collider.reset(newCollider);
        UpdateCollider();
    }
}

void Entity::UpdateCollider() {
    if (!m_collider) return;
    
    m_collider->Update();
    
    Vector2D currPos = m_collider->GetPosition();
    if (currPos.x != m_position.x || currPos.y != m_position.y) {
        m_collider->SetPosition(m_position);
    }
}

void Entity::SetRotation(float degrees) {
    m_rotation = degrees;
    
    while (m_rotation < 0) {
        m_rotation += 360.0f;
    }
    while (m_rotation >= 360.0f) {
        m_rotation -= 360.0f;
    }
}

float Entity::GetRotation() const {
    return m_rotation;
} 