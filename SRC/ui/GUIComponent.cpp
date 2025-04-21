#include <ui/GUIComponent.h>
#include <utils/Vector2D.h>

GUIComponent::GUIComponent(Renderer* renderer, AssetManager* assetManager, InputSystem* input)
    : m_renderer(renderer),
      m_assetManager(assetManager),
      m_input(input),
      m_isActive(true),
      m_isHovered(false) {
    
    m_rect = {0, 0, 0, 0};
}

GUIComponent::~GUIComponent() {
}

void GUIComponent::Update() {
    CheckMouseHover();
}

void GUIComponent::SetPosition(int x, int y) {
    m_rect.x = x;
    m_rect.y = y;
}

void GUIComponent::SetSize(int width, int height) {
    m_rect.w = width;
    m_rect.h = height;
}

void GUIComponent::SetActive(bool active) {
    m_isActive = active;
}

bool GUIComponent::IsActive() const {
    return m_isActive;
}

SDL_Rect GUIComponent::GetRect() const {
    return m_rect;
}

bool GUIComponent::IsMouseOver() const {
    return m_isHovered;
}

void GUIComponent::CheckMouseHover() {
    Vector2D mousePos;
    m_input->GetMouseState(mousePos);
    
    m_isHovered = (mousePos.x >= m_rect.x && 
                   mousePos.x <= m_rect.x + m_rect.w &&
                   mousePos.y >= m_rect.y && 
                   mousePos.y <= m_rect.y + m_rect.h);
} 