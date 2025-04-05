#pragma once

#include <ui/GUIComponent.h>
#include <functional>
#include <string>

class Button : public GUIComponent {
public:
    Button(Renderer* renderer, 
           AssetManager* assetManager, 
           InputSystem* input,
           const std::string& textureId, 
           std::function<void()> clickCallback);
    
    ~Button() override;
    
    // Core GUIComponent methods
    void Update() override;
    void Render() override;
    bool HandleEvent() override;
    
    // Button specific methods
    void SetTexture(const std::string& textureId);
    void SetHoverTexture(const std::string& textureId);
    void SetClickSound(const std::string& soundId);
    
private:
    std::string m_textureId;
    std::string m_hoverTextureId;
    std::string m_clickSoundId;
    std::function<void()> m_clickCallback;
    
    bool m_isPressed;
    bool m_wasHovered;
};