#include <ui/Label.h>
#include <iostream>
#include <sstream>

Label::Label(Renderer* renderer, 
             AssetManager* assetManager, 
             InputSystem* input,
             const std::string& text, 
             TTF_Font* font,
             const std::string& textureId,
             SDL_Color color)
    : GUIComponent(renderer, assetManager, input),
      m_text(text),
      m_font(font),
      m_color(color),
      m_textureId(textureId),
      m_hasCustomId(!textureId.empty()) {
    
    if (m_textureId.empty()) {
        m_textureId = GenerateTextureId();
    }
    
    RefreshTexture();
}

Label::~Label() {
    // Unload the texture from AssetManager if we're using a generated ID
    if (!m_hasCustomId) {
        m_assetManager->UnloadTexture(m_textureId);
    }
}

void Label::Update() {
    GUIComponent::Update();
    // Labels typically don't need additional update logic
}

void Label::Render() {
    if (!m_isActive) return;
    
    SDL_Texture* texture = m_assetManager->GetTexture(m_textureId);
    if (texture) {
        SDL_Rect destRect = m_rect;
        SDL_RenderCopy(m_renderer->GetSDLRenderer(), texture, nullptr, &destRect);
    }
}

bool Label::HandleEvent() {
    return false;
}

void Label::SetText(const std::string& text) {
    if (m_text != text) {
        m_text = text;
        RefreshTexture();
    }
}

void Label::SetColor(SDL_Color color) {
    if (m_color.r != color.r || m_color.g != color.g || m_color.b != color.b || m_color.a != color.a) {
        m_color = color;
        RefreshTexture();
    }
}

void Label::SetFont(TTF_Font* font) {
    if (m_font != font) {
        m_font = font;
        RefreshTexture();
    }
}

std::string Label::GetText() const {
    return m_text;
}

std::string Label::GenerateTextureId() const {
    // Create a unique ID for this label's texture
    std::stringstream ss;
    ss << "label_" << this << "_" << m_text;
    return ss.str();
}

void Label::RefreshTexture() {
    if (!m_font || m_text.empty()) {
        return;
    }
    
    SDL_Surface* textSurface = TTF_RenderText_Blended(m_font, m_text.c_str(), m_color);
    if (!textSurface) {
        std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        return;
    }
    
    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer->GetSDLRenderer(), textSurface);
    if (!texture) {
        std::cout << "Failed to create texture from rendered text: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }
    
    // Update label dimensions based on the text size
    m_rect.w = textSurface->w;
    m_rect.h = textSurface->h;
    
    // Free the surface, we no longer need it
    SDL_FreeSurface(textSurface);
    
    m_assetManager->UnloadTexture(m_textureId);
    
    m_assetManager->AddTexture(m_textureId, texture);
} 