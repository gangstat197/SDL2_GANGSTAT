#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <functional>
#include <memory>
#include "GUIComponent.h"

#include <managers/SoundManager.h>

class Button : public GUIComponent {
public:
    Button(Renderer* renderer, 
           AssetManager* assetManager, 
           InputSystem* input,
           const std::string& textureId, 
           std::function<void()> clickCallback);
    
    ~Button() override;
    
    void Update() override;
    void Render() override;
    bool HandleEvent() override;
    
    void SetTexture(const std::string& textureId);
    void SetHoverTexture(const std::string& textureId);
    void SetHoverSound(const std::string& soundId);
    void SetClickSound(const std::string& soundId);
    void SetPosition(int x, int y);
    void SetSize(int width, int height);
    
private:
    std::string m_textureId;
    std::string m_hoverTextureId;
    std::string m_hoverSoundId;
    std::string m_clickSoundId;
    std::function<void()> m_clickCallback;
    
    bool m_isPressed;
    bool m_wasHovered;
    
    float m_currentScale;
    float m_targetScale;
    float m_normalScale;
    float m_hoverScale;
    float m_scaleSpeed;
};