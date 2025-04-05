#pragma once

#include <SDL2/SDL.h>
#include <core/Renderer.h>
#include <managers/AssetManager.h>
#include <systems/InputSystem.h>
#include <string>
#include <functional>

class GUIComponent {
public:
    GUIComponent(Renderer* renderer, AssetManager* assetManager, InputSystem* input);
    virtual ~GUIComponent();

    virtual void Update();
    virtual void Render() = 0;
    virtual bool HandleEvent() = 0;
    
    void SetPosition(int x, int y);
    void SetSize(int width, int height);
    void SetActive(bool active);
    bool IsActive() const;
    
    SDL_Rect GetRect() const;
    bool IsMouseOver() const;

protected:
    Renderer* m_renderer;
    AssetManager* m_assetManager;
    InputSystem* m_input;
    
    SDL_Rect m_rect;
    bool m_isActive;
    bool m_isHovered;
    
    // Helper method to check if mouse is over component
    void CheckMouseHover();
};