#include <entities/Player.h>
#include <utils/Vector2D.h>
#include <iostream>

Player::Player(Renderer* renderer, AssetManager* assetManager, InputSystem* input, const std::string& textureId)
    : Entity(renderer, assetManager, textureId),
      m_input(input),
      m_health(1),
      m_isInvincible(false),
      m_invincibilityTimer(0.0f),
      m_invincibilityDuration(0.0f),
      m_sizeReductionTimer(0.0f),
      m_originalScale(1.0f),
      m_blinkTimer(0.0f),
      m_isVisible(true) {
    
    // Store original scale for power-up resets
    m_originalScale = m_scale;
}

Player::~Player() {
    // No specific cleanup needed
}

void Player::Update(float deltaTime) {
    if (!m_isActive) return;
    
    // Update player position based on mouse position
    Vector2D mousePos;
    m_input->GetMouseState(mousePos);
    SetPosition(mousePos);
    
    // Handle invincibility
    if (m_isInvincible) {
        m_invincibilityTimer += deltaTime;
        
        // Toggle visibility every 0.1 seconds for blinking effect
        m_blinkTimer += deltaTime;
        if (m_blinkTimer >= 0.1f) {
            m_isVisible = !m_isVisible;
            m_blinkTimer = 0.0f;
        }
        
        // Check if invincibility has expired
        if (m_invincibilityTimer >= m_invincibilityDuration) {
            m_isInvincible = false;
            m_isVisible = true;
        }
    }
    
    // Handle size reduction powerup
    if (m_sizeReductionTimer > 0.0f) {
        m_sizeReductionTimer -= deltaTime;
        
        // Reset size when timer expires
        if (m_sizeReductionTimer <= 0.0f) {
            ResetSize();
        }
    }
}

void Player::Render() {
    if (!m_isActive || !m_isVisible) return;
    
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
        
        // Set alpha for invincibility visual feedback
        if (m_isInvincible) {
            SDL_SetTextureAlphaMod(texture, 180); // Semi-transparent
        } else {
            SDL_SetTextureAlphaMod(texture, 255); // Fully opaque
        }
        
        SDL_RenderCopy(m_renderer->GetSDLRenderer(), texture, nullptr, &destRect);
    }
}

void Player::SetHealth(int health) {
    m_health = health;
}

int Player::GetHealth() const {
    return m_health;
}

void Player::TakeDamage(int damage) {
    if (m_isInvincible) return;
    
    m_health -= damage;
    if (m_health <= 0) {
        m_health = 0;
        m_isActive = false;
    } else {
        // Apply brief invincibility after taking damage
        SetInvincible(true, 1.0f);
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
    // Store original scale if not already in a reduced state
    if (m_sizeReductionTimer <= 0.0f) {
        m_originalScale = m_scale;
    }
    
    // Apply the size reduction
    m_scale = m_originalScale * reductionFactor;
    m_sizeReductionTimer = duration;
    
    // Update the collision box
    UpdateCollider();
}

void Player::ResetSize() {
    m_scale = m_originalScale;
    m_sizeReductionTimer = 0.0f;
    
    // Update the collision box
    UpdateCollider();
}
