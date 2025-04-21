#pragma once

#include <SDL2/SDL.h>
#include <core/Renderer.h>
#include <managers/AssetManager.h>
#include <utils/Vector2D.h>
#include <entities/base/Movement.h>
#include <entities/base/Rotation.h>
#include <entities/base/Collider.h>

#include <memory>

class Entity {
public:
    Entity(Renderer* renderer, AssetManager* assetManager, const std::string& textureId, ColliderType colliderType);
    virtual ~Entity();
    
    virtual void Update(float deltaTime);
    virtual void Render();
    
    void SetPosition(const Vector2D& position);
    void SetPosition(float x, float y);
    void SetInitialPosition(const Vector2D& position);
    
    Vector2D GetPosition() const;

    void SetScale(float scale);
    float GetScale() const;
    
    void SetSize(int width, int height);
    int GetWidth() const;
    int GetHeight() const;
    
    void SetRotation(float degrees);
    float GetRotation() const;
    
    void SetActive(bool active);
    bool IsActive() const;
    
    void RenderDebug();
    
    void SetNewCollider(Collider* newCollider);

    Movement* GetMovementComponent() const;
    Rotation* GetRotationComponent() const;
    Collider* GetColliderComponent() const;
    ColliderType GetColliderType() const;
    
    const std::string& GetTextureId() const { return m_textureId; }
    
    Renderer* GetRenderer() const { return m_renderer; }
protected:
    Renderer* m_renderer;
    AssetManager* m_assetManager;
    
    std::string m_textureId;
    Vector2D m_position;
    float m_scale;
    float m_rotation; 
    int m_width;
    int m_height;
    bool m_isActive;
    
    void UpdateCollider();
    
    std::unique_ptr<Movement> m_movement;
    std::unique_ptr<Rotation> m_rotationComponent;
    std::unique_ptr<Collider> m_collider;
}; 