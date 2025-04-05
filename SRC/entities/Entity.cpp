#include <entities/Entity.h>

Entity::Entity(Renderer* renderer, AssetManager* assetManager, const std::string& textureId)
    : m_renderer(renderer),
      m_assetManager(assetManager),
      m_textureId(textureId),
      m_position(0, 0),
      m_scale(1.0f),
      m_rotation(0.0f),
      m_width(0),
      m_height(0),
      m_isActive(true) {
    
    // Get texture dimensions if available
    SDL_Texture* texture = m_assetManager->GetTexture(textureId);
    if (texture) {
        SDL_QueryTexture(texture, nullptr, nullptr, &m_width, &m_height);
    }
    
    UpdateCollider();
}

Entity::~Entity() {}

void Entity::SetPosition(const Vector2D& position) {
    m_position = position;
    UpdateCollider();
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

SDL_Rect Entity::GetCollider() const {
    return m_collider;
}

bool Entity::CheckCollision(const SDL_Rect& other) const {
    return SDL_HasIntersection(&m_collider, &other);
}

void Entity::SetActive(bool active) {
    m_isActive = active;
}

bool Entity::IsActive() const {
    return m_isActive;
}

void Entity::UpdateCollider() {
    // Update the collider based on position and size
    int scaledWidth = static_cast<int>(m_width * m_scale);
    int scaledHeight = static_cast<int>(m_height * m_scale);
    
    m_collider = {
        static_cast<int>(m_position.x - scaledWidth / 2),  // Center the collider on position
        static_cast<int>(m_position.y - scaledHeight / 2),
        scaledWidth,
        scaledHeight
    };
}

void Entity::SetRotation(float degrees) {
    m_rotation = degrees;
    
    // Normalize rotation to 0-360 range
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