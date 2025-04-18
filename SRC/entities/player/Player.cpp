#include <entities/player/Player.h>
#include <utils/Vector2D.h>
#include <iostream>

Player::Player(Renderer* renderer, AssetManager* assetManager, InputSystem* input, const std::string& textureId)
    : Entity(renderer, assetManager, textureId, ColliderType::CIRCLE),
      m_input(input),
      m_isInvincible(false),
      m_invincibilityTimer(0.0f),
      m_invincibilityDuration(0.0f),
      m_sizeReductionTimer(0.0f),
      m_originalScale(1.0f),
      m_blinkTimer(0.0f),
      m_isVisible(true) {
    
    m_originalScale = m_scale;
}

Player::~Player() {
}

void Player::Update(float deltaTime) {
    if (!IsActive()) return;
    
    Vector2D mousePos;
    m_input->GetMouseState(mousePos);
    SetPosition(mousePos);
    
    if (m_isInvincible) {
        m_invincibilityTimer += deltaTime;
        
        m_blinkTimer += deltaTime;
        if (m_blinkTimer >= 0.1f) {
            m_isVisible = !m_isVisible;
            m_blinkTimer = 0.0f;
        }
        
        if (m_invincibilityTimer >= m_invincibilityDuration) {
            m_isInvincible = false;
            m_isVisible = true;
        }
    }
    
    if (m_sizeReductionTimer > 0.0f) {
        m_sizeReductionTimer -= deltaTime;
        
        if (m_sizeReductionTimer <= 0.0f) {
            ResetSize();
        }
    }
    
    Entity::Update(deltaTime);
}

void Player::Render() {
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
}

bool Player::IsInvincible() const {
    return m_isInvincible;
}

void Player::SetInvincible(bool invincible, float duration) {
    m_isInvincible = invincible;
    
    if (invincible) {
        m_invincibilityTimer = 0.0f;
        m_invincibilityDuration = duration;
        m_isVisible = true;
        m_blinkTimer = 0.0f;
    }
}

void Player::ApplySizeReduction(float reductionFactor, float duration) {
    if (m_sizeReductionTimer <= 0.0f) {
        m_originalScale = m_scale;
    }
    
    m_scale = m_originalScale * reductionFactor;
    m_sizeReductionTimer = duration;
    
    UpdateCollider();
}

void Player::ResetSize() {
    m_scale = m_originalScale;
    m_sizeReductionTimer = 0.0f;
    
    UpdateCollider();
}
