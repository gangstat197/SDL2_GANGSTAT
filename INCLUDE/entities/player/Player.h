#pragma once

#include <entities/base/Entity.h>
#include <systems/InputSystem.h>
#include <managers/SoundManager.h>

class Player : public Entity {
public:
    Player(Renderer* renderer, AssetManager* assetManager, InputSystem* input, SoundManager* soundManager, const std::string& textureId);
    ~Player() override;
    
    void Update(float deltaTime) override;
    void Render() override;
    
    bool IsInvincible() const;
    void SetInvincible(bool invincible, float duration = 0.0f);

    void Hit();
    
    void ApplySizeReduction(float reductionFactor, float duration);
    void ResetSize();
    
private:
    InputSystem* m_input;
    SoundManager* m_soundManager;
    bool m_isInvincible;
    float m_invincibilityTimer;
    float m_invincibilityDuration;
    
    float m_sizeReductionTimer;
    float m_originalScale;
    
    float m_blinkTimer;
    bool m_isVisible;

    bool m_isHit;
    float m_hitDuration;
    float m_hitTimer;
};
