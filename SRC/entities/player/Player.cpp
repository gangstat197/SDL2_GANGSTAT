#include <entities/player/Player.h>
#include <utils/Vector2D.h>
#include <utils/GameSettings.h>
#include <iostream>

Player::Player(Renderer* renderer, AssetManager* assetManager, InputSystem* input, SoundManager* soundManager, const std::string& textureId)
    : Entity(renderer, assetManager, textureId, ColliderType::CIRCLE),
      m_input(input),
      m_soundManager(soundManager),
      m_isInvincible(false),
      m_invincibilityDuration(0.0f),
      m_sizeReductionTimer(0.0f),
      m_originalScale(1.0f),
      m_wasScaled(false),
      m_blinkTimer(0.0f),
      m_isVisible(true),
      m_isHit(false),
      m_hitDuration(0.0f),
      m_hitTimer(0.0f),
      m_maxTrailLength(10),
      m_trailUpdateTime(0.02f),
      m_trailTimer(0.0f) {
    
    m_originalScale = m_scale;
    SaveOriginalColliderData();
}

Player::~Player() {
}

void Player::Update(float deltaTime) {
    if (!IsActive()) return;
    
    Vector2D mousePos;
    m_input->GetMouseState(mousePos);
    SetPosition(mousePos);
    
    UpdateTrail(deltaTime);
    
    if (m_isInvincible) {
        if (m_invincibilityTimer.GetSeconds() >= m_invincibilityDuration) {
            m_isInvincible = false;
            m_isVisible = true;
        } else {
            m_blinkTimer += deltaTime;
            if (m_blinkTimer >= 0.1f) {
                m_isVisible = !m_isVisible;
                m_blinkTimer = 0.0f;
            }
        }
    }
    
    if (m_sizeReductionTimer > 0.0f) {
        m_sizeReductionTimer -= deltaTime;
        
        if (m_sizeReductionTimer <= 0.0f) {
            ResetSize();
        }
    }

    if (m_isHit) {
        m_hitTimer += deltaTime;
        if (m_hitTimer >= m_hitDuration) {
            m_isHit = false;
        }
    }

    Entity::Update(deltaTime);
}

void Player::Render() {
    RenderTrail();
    
    if (!IsActive() || !m_isVisible) return;
    
    if (m_isVisible) {
        SDL_Texture* texture = m_assetManager->GetTexture(m_textureId);
        if (texture) {

            if (m_isInvincible) {
                SDL_SetTextureAlphaMod(texture, 180); 
            } else {
                SDL_SetTextureAlphaMod(texture, 255); 
            }
            
            Entity::Render();
        }
    }

    if (m_isHit && m_assetManager) {
        SDL_Texture* hit_vignette = m_assetManager->GetTexture("hit_vignette");
        if (hit_vignette && m_renderer) {
            SDL_RenderCopy(m_renderer->GetSDLRenderer(), hit_vignette, NULL, NULL);
        }
    }
}

void Player::UpdateTrail(float deltaTime) {
    m_trailTimer += deltaTime;
    
    float downwardMovement = GameSettings::BACKGROUND_SCROLL_SPEED * deltaTime;
    for (auto& pos : m_trailPositions) {
        pos.y += downwardMovement * 2;
    }
    
    if (m_trailTimer >= m_trailUpdateTime) {
        m_trailTimer = 0.0f;
        
        m_trailPositions.push_front(m_position);
        
        if (m_trailPositions.size() > m_maxTrailLength) {
            m_trailPositions.pop_back();
        }
    }
}

void Player::RenderTrail() {
    if (m_trailPositions.empty()) return;
    
    SDL_Renderer* sdlRenderer = m_renderer->GetSDLRenderer();
    SDL_Texture* texture = m_assetManager->GetTexture(m_textureId);
    
    if (!texture) return;
    
    
    int width = m_width * m_scale;
    int height = m_height * m_scale;
    
    for (size_t i = 0; i < m_trailPositions.size(); i++) {
        float fadeFactor = 1.0f - (static_cast<float>(i) / m_trailPositions.size());
        fadeFactor = fadeFactor * fadeFactor; 

        Uint8 alpha = static_cast<Uint8>(fadeFactor * 160); 
        
        float sizeFactor = 0.95f - (0.55f * (static_cast<float>(i) / m_trailPositions.size()));
        int trailWidth = static_cast<int>(width * sizeFactor);
        int trailHeight = static_cast<int>(height * sizeFactor);
        
        // Tint color 
        Uint8 r = 255; 
        Uint8 g = 255;
        Uint8 b = 255;
        
        if (i > 2) { 
            float colorFade = static_cast<float>(i - 2) / (m_trailPositions.size() - 2);
            r = static_cast<Uint8>(255 - (colorFade * 55));
            g = static_cast<Uint8>(255 - (colorFade * 55)); 
            b = 255;  
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

bool Player::IsInvincible() const {
    return m_isInvincible;
}

void Player::SetInvincible(bool invincible, float duration) {
    m_isInvincible = invincible;
    
    if (invincible) {
        m_invincibilityTimer.Start();
        m_invincibilityDuration = duration;
        m_isVisible = true;
        m_blinkTimer = 0.0f;
    }
}

void Player::ApplySizeReduction(float reductionFactor, float duration) {
    if (m_sizeReductionTimer <= 0.0f) {
        m_originalScale = m_scale;
        SaveOriginalColliderData();
        m_wasScaled = true;
    }
    
    SetScale(m_originalScale * reductionFactor);
    m_sizeReductionTimer = duration;
}

void Player::ResetSize() {
    SetScale(m_originalScale);
    m_sizeReductionTimer = 0.0f;
    
    if (m_wasScaled) {
        RestoreOriginalCollider();
        m_wasScaled = false;
    }
}

void Player::SaveOriginalColliderData() {
    if (m_collider) {
        m_originalColliderType = m_collider->GetColliderType();
        m_originalRadius = m_collider->GetRadius();
        m_originalColliderPoints = m_collider->GetPolygonPoints();
    }
}

void Player::RestoreOriginalCollider() {
    if (m_collider) {
        if (m_originalColliderType == ColliderType::CIRCLE) {
            m_collider->SetCircleCollider(m_originalRadius);
        } else if (m_originalColliderType == ColliderType::POLYGON) {
            m_collider->SetPolygonCollider(m_originalColliderPoints.size(), m_originalColliderPoints);
        }
        UpdateCollider();
    }
}

void Player::Hit() {
    if (!m_isInvincible) {
        SetInvincible(true, 1.0f);
        
        if (m_soundManager) {
            m_soundManager->PlaySound("player_hit");
        }
        
        m_isHit = true;
        m_hitDuration = 0.5f;
        m_hitTimer = 0.0f;
    }
}