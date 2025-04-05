#pragma once

#include <ui/GUIComponent.h>
#include <string>
#include <SDL2/SDL_ttf.h>

class Label : public GUIComponent {
public:
    Label(Renderer* renderer, 
          AssetManager* assetManager, 
          InputSystem* input,
          const std::string& text, 
          TTF_Font* font,
          const std::string& textureId = "",
          SDL_Color color = {255, 255, 255, 255});
    
    ~Label() override;
    
    // Core GUIComponent methods
    void Update() override;
    void Render() override;
    bool HandleEvent() override; // Labels don't handle events by default
    
    // Label specific methods
    void SetText(const std::string& text);
    void SetColor(SDL_Color color);
    void SetFont(TTF_Font* font);
    std::string GetText() const;
    
private:
    std::string m_text;
    TTF_Font* m_font;
    SDL_Color m_color;
    std::string m_textureId;
    bool m_hasCustomId;
    
    // Helper method to refresh the texture when text changes
    void RefreshTexture();
    
    // Generate a unique texture ID if none provided
    std::string GenerateTextureId() const;
}; 