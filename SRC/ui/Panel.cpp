#include <ui/Panel.h>
#include <algorithm>

Panel::Panel(Renderer* renderer, 
             AssetManager* assetManager, 
             InputSystem* input,
             const std::string& backgroundTexture)
    : GUIComponent(renderer, assetManager, input),
      m_backgroundTextureId(backgroundTexture),
      m_backgroundColor({0, 0, 0, 255}),
      m_hasBackgroundColor(false) {
    
    // Set default size if not using a texture
    if (m_backgroundTextureId.empty()) {
        SetSize(200, 200);
    } else {
        // Set size based on texture
        SDL_Texture* texture = m_assetManager->GetTexture(backgroundTexture);
        if (texture) {
            int width, height;
            SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
            SetSize(width, height);
        } else {
            SetSize(200, 200);
        }
    }
}

Panel::~Panel() {
    ClearChildren();
}

void Panel::Update() {
    GUIComponent::Update();
    
    // Update all children
    for (auto& child : m_children) {
        child->Update();
    }
}

void Panel::Render() {
    if (!m_isActive) return;
    
    // Render background
    if (!m_backgroundTextureId.empty()) {
        SDL_Texture* texture = m_assetManager->GetTexture(m_backgroundTextureId);
        if (texture) {
            m_renderer->RenderTexture(texture, m_rect.x, m_rect.y);
        }
    } else if (m_hasBackgroundColor) {
        // Save current render draw color
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(m_renderer->GetSDLRenderer(), &r, &g, &b, &a);
        
        // Set panel background color
        SDL_SetRenderDrawColor(m_renderer->GetSDLRenderer(), 
                               m_backgroundColor.r, 
                               m_backgroundColor.g, 
                               m_backgroundColor.b, 
                               m_backgroundColor.a);
        
        // Draw filled rectangle
        SDL_RenderFillRect(m_renderer->GetSDLRenderer(), &m_rect);
        
        // Restore previous draw color
        SDL_SetRenderDrawColor(m_renderer->GetSDLRenderer(), r, g, b, a);
    }
    
    // Render all children
    for (auto& child : m_children) {
        child->Render();
    }
}

bool Panel::HandleEvent() {
    if (!m_isActive) return false;
    
    // Check if mouse is over panel first
    if (!m_isHovered) {
        return false;
    }
    
    // Handle events for children in reverse order (top to bottom)
    // This gives priority to components on top
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        if ((*it)->HandleEvent()) {
            return true; // Event was handled by a child
        }
    }
    
    // Panel itself doesn't handle events by default
    return false;
}

void Panel::AddChild(std::shared_ptr<GUIComponent> child) {
    m_children.push_back(child);
}

void Panel::RemoveChild(std::shared_ptr<GUIComponent> child) {
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        m_children.erase(it);
    }
}

void Panel::ClearChildren() {
    m_children.clear();
}

void Panel::SetBackgroundTexture(const std::string& textureId) {
    m_backgroundTextureId = textureId;
    m_hasBackgroundColor = false;
    
    if (!textureId.empty()) {
        SDL_Texture* texture = m_assetManager->GetTexture(textureId);
        if (texture) {
            int width, height;
            SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
            SetSize(width, height);
        }
    }
}

void Panel::SetBackgroundColor(SDL_Color color) {
    m_backgroundColor = color;
    m_hasBackgroundColor = true;
}

void Panel::SetBackgroundAlpha(Uint8 alpha) {
    m_backgroundColor.a = alpha;
} 