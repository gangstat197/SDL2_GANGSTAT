#pragma once

#include <entities/base/Entity.h>
#include <systems/InputSystem.h>

class Player : public Entity {
public:
    Player(Renderer* renderer, AssetManager* assetManager, InputSystem* input, const std::string& textureId);
    ~Player() override;
    
    void Update(float deltaTime) override;
    void Render() override;
    
    bool IsInvincible() const;
    void SetInvincible(bool invincible, float duration = 0.0f);
    
    void ApplySizeReduction(float reductionFactor, float duration);
    void ResetSize();
    
private:
    InputSystem* m_input;
    bool m_isInvincible;
    float m_invincibilityTimer;
    float m_invincibilityDuration;
    
    float m_sizeReductionTimer;
    float m_originalScale;
    
    float m_blinkTimer;
    bool m_isVisible;
};
