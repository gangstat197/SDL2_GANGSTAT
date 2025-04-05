#include <entities/Enemy.h>
#include <entities/Player.h>
#include <cmath>
#include <iostream>
#include <random>

Enemy::Enemy(Renderer* renderer, AssetManager* assetManager, const std::string& textureId)
    : Entity(renderer, assetManager, textureId),
      m_speed(100.0f),
      m_damage(1),
      m_movementPattern(MovementPattern::STRAIGHT),
      m_elapsedTime(0.0f),
      m_colliderType(ColliderType::RECTANGLE),
      m_showCollider(false),
      m_rotationSpeed(0.0f) {
    
    m_initialPosition = m_position;
    
    // Initialize circle collider
    m_circleCollider.center = m_position;
    m_circleCollider.radius = std::min(m_width, m_height) * 0.5f * m_scale;
    
    // Initialize polygon collider with a default triangle
    SetPolygonCollider(3, 0.5f);
    
    // Set a random rotation speed (-30 to +30 degrees per second)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-30.0f, 30.0f);
    m_rotationSpeed = dist(gen);
}

Enemy::~Enemy() {
}

void Enemy::Update(float deltaTime) {
    if (!m_isActive) return;
    
    m_elapsedTime += deltaTime;
    
    // Update movement based on pattern
    switch (m_movementPattern) {
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
        case MovementPattern::CUSTOM:
            if (m_customMoveFunction) {
                m_customMoveFunction(this, deltaTime);
            }
            break;
    }
    
    // Calculate rotation based on rotation speed
    if (m_rotationSpeed != 0.0f) {
        m_rotation += m_rotationSpeed * deltaTime;
        // Keep rotation within 0-360 degrees
        if (m_rotation > 360.0f) {
            m_rotation -= 360.0f;
        } else if (m_rotation < 0.0f) {
            m_rotation += 360.0f;
        }
    }
    
    // Update rectangle bounds based on new position
    UpdateCollider();
    
    // Check if enemy is off-screen (below the bottom edge)
    if (m_position.y > 850) {
        m_isActive = false;
    }
}

void Enemy::Render() {
    if (!m_isActive) return;
    
    SDL_Texture* texture = m_assetManager->GetTexture(m_textureId);
    if (texture) {
        // Calculate destination rectangle based on position and scale
        int scaledWidth = static_cast<int>(m_width * m_scale);
        int scaledHeight = static_cast<int>(m_height * m_scale);
        
        SDL_Rect destRect = {
            static_cast<int>(m_position.x - scaledWidth / 2),
            static_cast<int>(m_position.y - scaledHeight / 2),
            scaledWidth,
            scaledHeight
        };
        
        // Create rotation point (center of the texture)
        SDL_Point center = {scaledWidth / 2, scaledHeight / 2};
        
        // Render with rotation
        SDL_RenderCopyEx(
            m_renderer->GetSDLRenderer(),
            texture,
            nullptr,           // Source rectangle (entire texture)
            &destRect,         // Destination rectangle
            m_rotation,        // Angle in degrees
            &center,           // Rotation point (center)
            SDL_FLIP_NONE      // No flipping
        );
        
        // Debug render for colliders if enabled
        if (m_showCollider) {
            RenderColliderDebug();
        }
    }
}

void Enemy::SetSpeed(float speed) {
    m_speed = speed;
}

float Enemy::GetSpeed() const {
    return m_speed;
}

void Enemy::SetDamage(int damage) {
    m_damage = damage;
}

int Enemy::GetDamage() const {
    return m_damage;
}

void Enemy::SetMovementPattern(MovementPattern pattern) {
    m_movementPattern = pattern;
    m_elapsedTime = 0.0f; // Reset time when pattern changes
}

void Enemy::SetCustomMovementFunction(std::function<void(Enemy*, float)> moveFunction) {
    m_customMoveFunction = moveFunction;
    m_movementPattern = MovementPattern::CUSTOM;
}

bool Enemy::CheckCollisionWithPlayer(Player* player) const {
    if (!m_isActive || !player || !player->IsActive() || player->IsInvincible()) {
        return false;
    }
    
    bool collision = false;
    
    switch (m_colliderType) {
        case ColliderType::RECTANGLE:
            collision = CheckCollision(player->GetCollider());
            break;
            
        case ColliderType::CIRCLE:
        {
            // Create a circle collider for the player
            CollisionCircle playerCircle = CollisionHandler::CreateCircleFromEntity(*player);
            collision = CollisionHandler::CheckCircleCollision(m_circleCollider, playerCircle);
            break;
        }
            
        case ColliderType::POLYGON:
        {
            // For polygon collision, first check against the player's rectangle for efficiency
            if (CheckCollision(player->GetCollider())) {
                // Then do point-in-polygon test for player's center
                collision = CollisionHandler::IsPointInPolygon(player->GetPosition(), m_polygonCollider);
            }
            break;
        }
    }
    
    return collision;
}

// New collider methods

void Enemy::SetColliderType(ColliderType type) {
    m_colliderType = type;
    UpdateCustomColliders();
}

ColliderType Enemy::GetColliderType() const {
    return m_colliderType;
}

void Enemy::SetCircleCollider(float radiusScale) {
    m_circleCollider.center = m_position;
    // Use the smaller dimension for a better fit
    float smallerDimension = std::min(m_width, m_height);
    m_circleCollider.radius = smallerDimension * radiusScale * m_scale;
    
    m_colliderType = ColliderType::CIRCLE;
}

CollisionCircle Enemy::GetCircleCollider() const {
    return m_circleCollider;
}

void Enemy::SetPolygonCollider(const std::vector<Vector2D>& points) {
    if (points.size() < 3) {
        // Need at least 3 points for a valid polygon
        return;
    }
    
    m_polygonCollider.points = points;
    m_colliderType = ColliderType::POLYGON;
    
    // Adjust points to current position and scale
    UpdateCustomColliders();
}

void Enemy::SetPolygonCollider(int sides, float radiusScale) {
    if (sides < 3) {
        // Need at least 3 sides for a valid polygon
        sides = 3;
    }
    
    std::vector<Vector2D> points;
    float radius = std::min(m_width, m_height) * radiusScale * 0.5f;
    
    // Create a regular polygon
    for (int i = 0; i < sides; i++) {
        float angle = (2.0f * M_PI * i) / sides;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        points.push_back(Vector2D(x, y)); // Points are relative to center (0,0)
    }
    
    m_polygonCollider.points = points;
    m_colliderType = ColliderType::POLYGON;
    
    // Adjust points to current position and scale
    UpdateCustomColliders();
}

CollisionPolygon Enemy::GetPolygonCollider() const {
    return m_polygonCollider;
}

void Enemy::RenderCollider(bool showCollider) {
    m_showCollider = showCollider;
}

// Movement pattern implementations
void Enemy::MoveStraight(float deltaTime) {
    // Simple downward movement
    m_position.y += m_speed * deltaTime;
    UpdateCollider();
}

void Enemy::MoveZigZag(float deltaTime) {
    // Move downward with horizontal zigzag
    m_position.y += m_speed * deltaTime;
    
    // Calculate horizontal zigzag movement
    float amplitude = 50.0f; // Width of zigzag pattern
    float frequency = 2.0f;  // Speed of zigzag pattern
    
    m_position.x = m_initialPosition.x + amplitude * sin(frequency * m_elapsedTime);
    
    UpdateCollider();
}

void Enemy::MoveCurve(float deltaTime) {
    // Move in a curved path
    m_position.y += m_speed * deltaTime;
    
    // Calculate curve based on sin function
    float amplitude = 100.0f; // Width of curve
    float frequency = 1.0f;   // Speed of curve movement
    
    m_position.x = m_initialPosition.x + amplitude * sin(frequency * m_elapsedTime);
    
    UpdateCollider();
}

void Enemy::MoveSpiral(float deltaTime) {
    // Move in a spiral pattern
    float radius = 30.0f * m_elapsedTime; // Increasing radius
    float angle = 5.0f * m_elapsedTime;   // Increasing angle
    
    // Spiral motion
    m_position.x = m_initialPosition.x + radius * cos(angle);
    m_position.y = m_initialPosition.y + radius * sin(angle) + m_speed * deltaTime;
    
    UpdateCollider();
}

// Private helper methods

void Enemy::UpdateCustomColliders() {
    m_circleCollider.center = m_position;
    
    CollisionPolygon updatedPolygon;
    for (const auto& point : m_polygonCollider.points) {
        Vector2D scaledPoint = {
            point.x * m_scale + m_position.x,
            point.y * m_scale + m_position.y
        };
        updatedPolygon.points.push_back(scaledPoint);
    }
    m_polygonCollider = updatedPolygon;
}

// Debug rendering of colliders - THIS IS FOR DEBUGGING ONLY (coded by Cursor)
void Enemy::RenderColliderDebug() {
    SDL_Renderer* sdlRenderer = m_renderer->GetSDLRenderer();
    
    // Variables used in multiple cases should be declared before the switch
    const int segments = 20; // For circle approximation
    
    // Set color based on collider type
    switch (m_colliderType) {
        case ColliderType::RECTANGLE:
            SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 128); // Red for rectangle
            SDL_RenderDrawRect(sdlRenderer, &m_collider);
            break;
            
        case ColliderType::CIRCLE:
            SDL_SetRenderDrawColor(sdlRenderer, 0, 255, 0, 128); // Green for circle
            
            // Draw circle approximation
            for (int i = 0; i < segments; i++) {
                float angle1 = (2.0f * M_PI * i) / segments;
                float angle2 = (2.0f * M_PI * (i + 1)) / segments;
                
                int x1 = m_circleCollider.center.x + m_circleCollider.radius * cos(angle1);
                int y1 = m_circleCollider.center.y + m_circleCollider.radius * sin(angle1);
                int x2 = m_circleCollider.center.x + m_circleCollider.radius * cos(angle2);
                int y2 = m_circleCollider.center.y + m_circleCollider.radius * sin(angle2);
                
                SDL_RenderDrawLine(sdlRenderer, x1, y1, x2, y2);
            }
            break;
            
        case ColliderType::POLYGON:
            SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 255, 128); // Blue for polygon
            
            // Draw polygon lines
            const auto& points = m_polygonCollider.points;
            for (size_t i = 0; i < points.size(); i++) {
                size_t nextIndex = (i + 1) % points.size();
                SDL_RenderDrawLine(
                    sdlRenderer,
                    static_cast<int>(points[i].x),
                    static_cast<int>(points[i].y),
                    static_cast<int>(points[nextIndex].x),
                    static_cast<int>(points[nextIndex].y)
                );
            }
            break;
    }
    
    // Reset render color
    SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
}

void Enemy::SetRotationSpeed(float degreesPerSecond) {
    m_rotationSpeed = degreesPerSecond;
}

float Enemy::GetRotationSpeed() const {
    return m_rotationSpeed;
}

void Enemy::UpdateMovementPattern(float deltaTime) {
    m_elapsedTime += deltaTime;
    
    // Update movement based on pattern
    switch (m_movementPattern) {
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
        case MovementPattern::CUSTOM:
            if (m_customMoveFunction) {
                m_customMoveFunction(this, deltaTime);
            }
            break;
    }
}

void Enemy::CheckOffScreen() {
    // Check if enemy is off-screen and should be removed
    // Default: remove if it goes below the screen
    if (m_position.y > 850) {
        m_isActive = false;
    }
    
    // Optional: also check other screen edges if needed
    const int screenWidth = 800; // Adjust according to your game's screen width
    
    if (m_position.x < -100 || m_position.x > screenWidth + 100) {
        m_isActive = false;
    }
}

void Enemy::UpdateCollider() {
    // Update rectangle collider from Entity base class
    m_collider = {
        static_cast<int>(m_position.x - (m_width * m_scale) / 2),
        static_cast<int>(m_position.y - (m_height * m_scale) / 2),
        static_cast<int>(m_width * m_scale),
        static_cast<int>(m_height * m_scale)
    };
    
    // Update custom colliders
    UpdateCustomColliders();
}
