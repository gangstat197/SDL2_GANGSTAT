#pragma once

#include <ui/GUIComponent.h>
#include <vector>
#include <memory>

class UIManager {
public:
    UIManager(Renderer* renderer, AssetManager* assetManager, InputSystem* input);
    ~UIManager();
    
    // Add a component to the UI (takes ownership)
    void AddComponent(std::shared_ptr<GUIComponent> component);
    
    // Remove a component from the UI
    void RemoveComponent(std::shared_ptr<GUIComponent> component);
    
    // Clear all components
    void Clear();
    
    // Update all components
    void Update();
    
    // Render all components
    void Render();
    
    // Handle events for all components
    void HandleEvents();
    
private:
    Renderer* m_renderer;
    AssetManager* m_assetManager;
    InputSystem* m_input;
    
    std::vector<std::shared_ptr<GUIComponent>> m_components;
};
