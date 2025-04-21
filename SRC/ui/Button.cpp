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
      m_wasHovered(false),
      m_currentScale(1.0f),
      m_targetScale(1.0f),
      m_normalScale(1.0f),
      m_hoverScale(1.2f), 
      m_scaleSpeed(10.0f)  
{
    SDL_Texture* texture = m_assetManager->GetTexture(textureId);
    if (texture) {
        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        SetSize(width, height);
    } else {
        std::cout << "Warning: Button created with invalid texture ID: " << textureId << std::endl;
        SetSize(100, 30); 
    }
}

Button::~Button() {
}

void Button::Update() {
    GUIComponent::Update();
    
    if (m_isHovered && !m_wasHovered && !m_hoverSoundId.empty()) {
        SoundManager::Instance().PlaySound(m_hoverSoundId);
    }

    bool hoverChanged = m_isHovered != m_wasHovered;
    m_wasHovered = m_isHovered;
    
    m_targetScale = m_isHovered ? m_hoverScale : m_normalScale;
    
    float deltaTime = 1.0f / 60.0f; 
    m_currentScale += (m_targetScale - m_currentScale) * m_scaleSpeed * deltaTime;
}

void Button::Render() {
    if (!m_isActive) return;
    
    std::string textureToRender = m_textureId;
    
    if (m_isHovered && !m_hoverTextureId.empty()) {
        textureToRender = m_hoverTextureId;
    }
    
    SDL_Texture* texture = m_assetManager->GetTexture(textureToRender);
    if (texture) {
        int scaledWidth = static_cast<int>(m_rect.w * m_currentScale);
        int scaledHeight = static_cast<int>(m_rect.h * m_currentScale);
        
        int offsetX = (m_rect.w - scaledWidth) / 2;
        int offsetY = (m_rect.h - scaledHeight) / 2;
        
        SDL_Rect destRect = {
            m_rect.x + offsetX,
            m_rect.y + offsetY,
            scaledWidth,
            scaledHeight
        };
        
        m_renderer->RenderTexture(texture, destRect);
    }
}

bool Button::HandleEvent() {
    if (!m_isActive) return false;
    
    if (m_isHovered && m_input->IsMouseButtonJustPressed(SDL_BUTTON_LEFT)) {
        m_isPressed = true;
        if (!m_clickSoundId.empty()) {
            SoundManager::Instance().PlaySound(m_clickSoundId);
        }
        return true;
    }
    
    if (m_isPressed && m_input->IsMouseButtonJustReleased(SDL_BUTTON_LEFT)) {
        m_isPressed = false;
        
        if (m_isHovered && m_clickCallback) {
            m_clickCallback();
            return true;
        }
    }
    
    return false;
}

void Button::SetTexture(const std::string& textureId) {
    m_textureId = textureId;
    
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

void Button::SetHoverSound(const std::string& soundId) {
    m_hoverSoundId = soundId;
}

void Button::SetClickSound(const std::string& soundId) {
    m_clickSoundId = soundId;
}

void Button::SetPosition(int x, int y) {
    GUIComponent::SetPosition(x, y);
}

void Button::SetSize(int width, int height) {
    GUIComponent::SetSize(width, height);
}