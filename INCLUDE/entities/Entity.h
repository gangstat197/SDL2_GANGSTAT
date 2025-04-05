#pragma once

#include <SDL2/SDL.h>
#include <core/Renderer.h>
#include <managers/AssetManager.h>
#include <utils/Vector2D.h>

class Entity {
public:
    Entity(Renderer* renderer, AssetManager* assetManager, const std::string& textureId);
    virtual ~Entity();
    
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    
    // Position
    void SetPosition(const Vector2D& position);
    void SetPosition(float x, float y);
    Vector2D GetPosition() const;
    
    // Scale
    void SetScale(float scale);
    float GetScale() const;
    
    // Rotation (in degrees)
    void SetRotation(float degrees);
    float GetRotation() const;
    
    // Size
    void SetSize(int width, int height);
    int GetWidth() const;
    int GetHeight() const;
    
    // Collider
    SDL_Rect GetCollider() const;
    bool CheckCollision(const SDL_Rect& other) const;
    
    // Status
    void SetActive(bool active);
    bool IsActive() const;
    
protected:
    Renderer* m_renderer;
    AssetManager* m_assetManager;
    
    std::string m_textureId;
    Vector2D m_position;
    float m_scale;
    float m_rotation; // In degrees
    int m_width;
    int m_height;
    bool m_isActive;
    
    SDL_Rect m_collider;
    void UpdateCollider();
}; 