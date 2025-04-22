#include <entities/powerup/PowerUp.h>
#include <entities/player/Player.h>
#include <utils/GameSettings.h>
#include <cmath>
#include <iostream>
#include <managers/SoundManager.h>

PowerUp::PowerUp(Renderer* renderer, AssetManager* assetManager, const std::string& textureId, PowerUpType type)
    : Entity(renderer, assetManager, textureId, ColliderType::CIRCLE),
      m_powerUpType(type),
      m_maxTrailLength(8),
      m_trailUpdateTime(0.015f),
      m_trailTimer(0.0f) {
    
    GetMovementComponent()->SetInitialPosition(GetPosition());
    GetMovementComponent()->SetPattern(MovementPattern::STRAIGHT);
    GetRotationComponent()->SetRotationSpeed(30.0f); 
    
    int width = GetWidth();
    GetColliderComponent()->SetCircleCollider(width/2 - 10);
}

PowerUp::~PowerUp() {
}

void PowerUp::Update(float deltaTime) {
    if (!IsActive()) return;
    
    UpdateTrail(deltaTime);
    
    Entity::Update(deltaTime);
    
    CheckOffScreen();
}

void PowerUp::Render() {
    if (!IsActive()) return;
    
    RenderTrail();
    
    Entity::Render();
}

void PowerUp::UpdateTrail(float deltaTime) {
    m_trailTimer += deltaTime;
    
    // Gentle upward movement (opposite of player's trail)
    float upwardMovement = GameSettings::BACKGROUND_SCROLL_SPEED * 0.2f * deltaTime;
    for (auto& pos : m_trailPositions) {
        pos.y -= upwardMovement;
    }
    
    if (m_trailTimer >= m_trailUpdateTime) {
        m_trailTimer = 0.0f;
        
        // Only add position if moved enough
        if (m_trailPositions.empty() || 
            (m_position - m_trailPositions.front()).magnitude() > 1.0f) {
            m_trailPositions.push_front(m_position);
            
            if (m_trailPositions.size() > m_maxTrailLength) {
                m_trailPositions.pop_back();
            }
        }
    }
}

void PowerUp::RenderTrail() {
    if (m_trailPositions.empty()) return;
    
    SDL_Renderer* sdlRenderer = m_renderer->GetSDLRenderer();
    SDL_Texture* texture = m_assetManager->GetTexture(m_textureId);
    
    if (!texture) return;
    
    int width = m_width * m_scale;
    int height = m_height * m_scale;
    
    // Set blend mode for smooth rendering
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    
    Uint8 baseR = 255, baseG = 255, baseB = 255;
    
    switch (m_powerUpType) {
        case PowerUpType::INVINCIBLE:
            baseR = 255; baseG = 223; baseB = 0; 
            break;
        case PowerUpType::SIZE_REDUCTION:
            baseR = 0; baseG = 210; baseB = 255; 
            break;
        case PowerUpType::POINT_MULTIPLIER:
            baseR = 255; baseG = 100; baseB = 255; 
            break;
    }
    
    for (size_t i = 0; i < m_trailPositions.size(); i++) {
        float t = static_cast<float>(i) / m_trailPositions.size();
        float fadeFactor = 1.0f - (t * t);
        
        Uint8 alpha = static_cast<Uint8>(fadeFactor * 90);
        
        float sizeFactor = 0.95f - (0.5f * t);
        int trailWidth = static_cast<int>(width * sizeFactor);
        int trailHeight = static_cast<int>(height * sizeFactor);
        
        Uint8 r = baseR;
        Uint8 g = baseG;
        Uint8 b = baseB;

        if (i > 0) {
            float colorFade = t * 0.7f;
            r = static_cast<Uint8>(r + (255 - r) * colorFade);
            g = static_cast<Uint8>(g + (255 - g) * colorFade);
            b = static_cast<Uint8>(b + (255 - b) * colorFade);
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

void PowerUp::SetSpeed(float speed) {
    GetMovementComponent()->SetSpeed(speed);
}

float PowerUp::GetSpeed() const {
    return GetMovementComponent()->GetSpeed();
}

void PowerUp::SetRotationSpeed(float degreesPerSecond) {
    GetRotationComponent()->SetRotationSpeed(degreesPerSecond);
}

float PowerUp::GetRotationSpeed() const {
    return GetRotationComponent()->GetRotationSpeed();
}

bool PowerUp::CheckCollisionWithPlayer(Player* player) {
    if (!player || !player->IsActive()) return false;
    
    return GetColliderComponent()->CheckCollision(player->GetColliderComponent());
}

void PowerUp::ApplyEffect(Player* player) {
    if (!player) return;
    
    SoundManager::Instance().PlaySound("power_up");
    
    switch (m_powerUpType) {
        case PowerUpType::INVINCIBLE:
            player->SetInvincible(true, 5.0f);
            break;
            
        case PowerUpType::SIZE_REDUCTION:
            player->ApplySizeReduction(0.6f, 10.0f);
            break;
            
        case PowerUpType::POINT_MULTIPLIER:
            std::cout << "Point multiplier power-up collected" << std::endl;
            break;
    }
    
    SetActive(false);
}

PowerUpType PowerUp::GetPowerUpType() const {
    return m_powerUpType;
}

void PowerUp::CheckOffScreen() {
    GetMovementComponent()->CheckOffScreen(GameSettings::SCREEN_WIDTH, GameSettings::SCREEN_HEIGHT, 100);
} 