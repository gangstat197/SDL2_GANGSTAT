#include <ui/Button.h>
#include <iostream>

Button::Button(Renderer* renderer, 
               AssetManager* assetManager, 
               InputSystem* input,
               const std::string& textureId, 
               std::function<void()> clickCallback)
    : GUIComponent(renderer, assetManager, input),
      m_textureId(textureId),
      m_hoverTextureId(""),
      m_clickSoundId(""),
      m_clickCallback(clickCallback),
      m_isPressed(false),
      m_wasHovered(false) {
    
    // Set button size based on texture
    SDL_Texture* texture = m_assetManager->GetTexture(textureId);
    if (texture) {
        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        SetSize(width, height);
    } else {
        std::cout << "Warning: Button created with invalid texture ID: " << textureId << std::endl;
        SetSize(100, 30); // Default size if texture not available
    }
}

Button::~Button() {
}

void Button::Update() {
    GUIComponent::Update();
    
    bool hoverChanged = m_isHovered != m_wasHovered;
    m_wasHovered = m_isHovered;
    
    if (hoverChanged && m_isHovered) {
        // Play hover sound or trigger animation if needed
    }
}

void Button::Render() {
    if (!m_isActive) return;
    
    std::string textureToRender = m_textureId;
    
    if (m_isHovered && !m_hoverTextureId.empty()) {
        textureToRender = m_hoverTextureId;
    }
    
    SDL_Texture* texture = m_assetManager->GetTexture(textureToRender);
    if (texture) {
        m_renderer->RenderTexture(texture, m_rect.x, m_rect.y);
    }
}

bool Button::HandleEvent() {
    if (!m_isActive) return false;
    
    // Check for mouse button press
    if (m_isHovered && m_input->IsMouseButtonJustPressed(SDL_BUTTON_LEFT)) {
        m_isPressed = true;
        return true;
    }
    
    if (m_isPressed && m_input->IsMouseButtonJustReleased(SDL_BUTTON_LEFT)) {
        m_isPressed = false;
        
        // Only trigger if still hovering (user didn't drag away)
        if (m_isHovered && m_clickCallback) {
            // Play click sound if available
            // if (!m_clickSoundId.empty()) {
            //     // Play sound effect
            // }
            
            // Execute callback
            m_clickCallback();
            return true;
        }
    }
    
    return false;
}

void Button::SetTexture(const std::string& textureId) {
    m_textureId = textureId;
    
    // Update size based on new texture
    SDL_Texture* texture = m_assetManager->GetTexture(textureId);
    if (texture) {
        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        SetSize(width, height);
    }
}

void Button::SetHoverTexture(const std::string& textureId) {
    m_hoverTextureId = textureId;
}

void Button::SetClickSound(const std::string& soundId) {
    m_clickSoundId = soundId;
}