#pragma once

#include <entities/base/Entity.h>
#include <systems/InputSystem.h>
#include <managers/SoundManager.h>
#include <utils/Timer.h>
#include <deque>

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
    
    void SaveOriginalColliderData();
    
private:
    InputSystem* m_input;
    SoundManager* m_soundManager;
    bool m_isInvincible;
    Timer m_invincibilityTimer;
    float m_invincibilityDuration;
    
    float m_sizeReductionTimer;
    float m_originalScale;
    bool m_wasScaled;
    ColliderType m_originalColliderType;
    float m_originalRadius;
    std::vector<Vector2D> m_originalColliderPoints;
    
    float m_blinkTimer;
    bool m_isVisible;

    bool m_isHit;
    float m_hitDuration;
    float m_hitTimer;

    // Trail effect properties
    std::deque<Vector2D> m_trailPositions;
    int m_maxTrailLength;
    float m_trailUpdateTime;
    float m_trailTimer;
    
    void RenderTrail();
    void UpdateTrail(float deltaTime);
    void RestoreOriginalCollider();
};
