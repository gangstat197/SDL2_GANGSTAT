#pragma once

#include <entities/Entity.h>
#include <systems/InputSystem.h>

class Player : public Entity {
public:
    Player(Renderer* renderer, AssetManager* assetManager, InputSystem* input, const std::string& textureId);
    ~Player() override;
    
    void Update(float deltaTime) override;
    void Render() override;
    
    // Player-specific methods
    void SetHealth(int health);
    int GetHealth() const;
    void TakeDamage(int damage);
    bool IsInvincible() const;
    void SetInvincible(bool invincible, float duration = 0.0f);
    
    // Power-ups
    void ApplySizeReduction(float reductionFactor, float duration);
    void ResetSize();
    
private:
    InputSystem* m_input;
    int m_health;
    bool m_isInvincible;
    float m_invincibilityTimer;
    float m_invincibilityDuration;
    
    // Power-up related
    float m_sizeReductionTimer;
    float m_originalScale;
    
    // Animation/visual feedback
    float m_blinkTimer;
    bool m_isVisible;
};
