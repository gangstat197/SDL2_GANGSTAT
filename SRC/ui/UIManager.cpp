#include <ui/UIManager.h>
#include <algorithm>

UIManager::UIManager(Renderer* renderer, AssetManager* assetManager, InputSystem* input)
    : m_renderer(renderer), m_assetManager(assetManager), m_input(input) {
}

UIManager::~UIManager() {
    Clear();
}

void UIManager::AddComponent(std::shared_ptr<GUIComponent> component) {
    m_components.push_back(component);
}

void UIManager::RemoveComponent(std::shared_ptr<GUIComponent> component) {
    auto it = std::find(m_components.begin(), m_components.end(), component);
    if (it != m_components.end()) {
        m_components.erase(it);
    }
}

void UIManager::Clear() {
    m_components.clear();
}

void UIManager::Update() {
    for (auto& component : m_components) {
        component->Update();
    }
}

void UIManager::Render() {
    for (auto& component : m_components) {
        component->Render();
    }
}

void UIManager::HandleEvents() {
    for (auto it = m_components.rbegin(); it != m_components.rend(); ++it) {
        if ((*it)->HandleEvent()) {
            break;
        }
    }
}
