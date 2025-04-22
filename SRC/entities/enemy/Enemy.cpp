#include <entities/enemy/Enemy.h>
#include <entities/player/Player.h>
#include <utils/GameSettings.h>
#include <cmath>
#include <iostream>

Enemy::Enemy(Renderer* renderer, AssetManager* assetManager, const std::string& textureId, 
            ColliderType colliderType)
    : Entity(renderer, assetManager, textureId, colliderType),
      m_maxTrailLength(8),  // Slightly longer but still shorter than player
      m_trailUpdateTime(0.015f),  // Update more frequently for smoother trail
      m_trailTimer(0.0f) {
    
    GetMovementComponent()->SetInitialPosition(GetPosition());
    GetRotationComponent()->SetRotationSpeed(0);
}

Enemy::~Enemy() {
}

void Enemy::Update(float deltaTime) {
    if (!IsActive()) return;
    
    // Update the trail
    UpdateTrail(deltaTime);
    
    Entity::Update(deltaTime);
    
    CheckOffScreen();
}

void Enemy::Render() {
    if (!IsActive()) return;
    
    RenderTrail();
    
    Entity::Render();
}

void Enemy::UpdateTrail(float deltaTime) {
    m_trailTimer += deltaTime;
    
    float upwardMovement = GameSettings::BACKGROUND_SCROLL_SPEED * 0.2f * deltaTime;
    for (auto& pos : m_trailPositions) {
        pos.y -= upwardMovement; 
    }
    
    if (m_trailTimer >= m_trailUpdateTime) {
        m_trailTimer = 0.0f;
        
        if (m_trailPositions.empty() || 
            (m_position - m_trailPositions.front()).magnitude() > 1.0f) {
            m_trailPositions.push_front(m_position);
            
            if (m_trailPositions.size() > m_maxTrailLength) {
                m_trailPositions.pop_back();
            }
        }
    }
}

void Enemy::RenderTrail() {
    if (m_trailPositions.empty()) return;
    
    SDL_Renderer* sdlRenderer = m_renderer->GetSDLRenderer();
    SDL_Texture* texture = m_assetManager->GetTexture(m_textureId);
    
    if (!texture) return;
    
    int width = m_width * m_scale;
    int height = m_height * m_scale;

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    
    for (size_t i = 0; i < m_trailPositions.size(); i++) {
        float t = static_cast<float>(i) / m_trailPositions.size();
        float fadeFactor = 1.0f - (t * t);
        
        Uint8 alpha = static_cast<Uint8>(fadeFactor * 90);
        
        float sizeFactor = 0.9f - (0.7f * (static_cast<float>(i) / m_trailPositions.size()));
        int trailWidth = static_cast<int>(width * sizeFactor);
        int trailHeight = static_cast<int>(height * sizeFactor);
        
        Uint8 r = 200; 
        Uint8 g = 255;
        Uint8 b = 200;
        
        if (i > 1) {
            float colorFade = static_cast<float>(i - 1) / (m_trailPositions.size() - 1);
            r = static_cast<Uint8>(200 - (colorFade * 100));
            g = static_cast<Uint8>(255 - (colorFade * 50)); 
            b = static_cast<Uint8>(200 - (colorFade * 150));
        }
        
        SDL_SetTextureColorMod(texture, r, g, b);
        SDL_SetTextureAlphaMod(texture, alpha);
        
        int posX = static_cast<int>(m_trailPositions[i].x - (trailWidth / 2));
        int posY = static_cast<int>(m_trailPositions[i].y - (trailHeight / 2));
        
        SDL_Rect destRect = {posX, posY, trailWidth, trailHeight};
        SDL_RenderCopyEx(sdlRenderer, texture, NULL, &destRect, m_rotation, NULL, SDL_FLIP_NONE);
    }

    SDL_SetTextureColorMod(texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(texture, 255);
}

void Enemy::SetSpeed(float speed) {
    GetMovementComponent()->SetSpeed(speed);
}

float Enemy::GetSpeed() const {
    return GetMovementComponent()->GetSpeed();
}

void Enemy::SetRotationSpeed(float degreesPerSecond) {
    GetRotationComponent()->SetRotationSpeed(degreesPerSecond);
}

float Enemy::GetRotationSpeed() const {
    return GetRotationComponent()->GetRotationSpeed();
}

void Enemy::SetMovementPattern(MovementPattern pattern) {
    GetMovementComponent()->SetPattern(pattern);
}

bool Enemy::CheckCollisionWithPlayer(Player* player) {
    if (!player) return false;
    
    return GetColliderComponent()->CheckCollision(player->GetColliderComponent());
}

void Enemy::CheckOffScreen() {
    GetMovementComponent()->CheckOffScreen(800, 850, 100);
}
