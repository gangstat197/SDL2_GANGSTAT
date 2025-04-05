#pragma once

#include <ui/GUIComponent.h>
#include <vector>
#include <memory>

class Panel : public GUIComponent {
public:
    Panel(Renderer* renderer, 
          AssetManager* assetManager, 
          InputSystem* input,
          const std::string& backgroundTexture = "");
    
    ~Panel() override;
    
    // Core GUIComponent methods
    void Update() override;
    void Render() override;
    bool HandleEvent() override;
    
    // Panel specific methods
    void AddChild(std::shared_ptr<GUIComponent> child);
    void RemoveChild(std::shared_ptr<GUIComponent> child);
    void ClearChildren();
    
    void SetBackgroundTexture(const std::string& textureId);
    void SetBackgroundColor(SDL_Color color);
    void SetBackgroundAlpha(Uint8 alpha);
    
private:
    std::string m_backgroundTextureId;
    SDL_Color m_backgroundColor;
    bool m_hasBackgroundColor;
    
    std::vector<std::shared_ptr<GUIComponent>> m_children;
}; 