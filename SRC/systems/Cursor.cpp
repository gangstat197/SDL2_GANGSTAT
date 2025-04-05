#include <systems\Cursor.h>
#include <managers\AssetManager.h>
#include <systems\InputSystem.h>
#include <utils\Vector2D.h>
#include <SDL2/SDL.h>
#include <iostream>

Cursor::Cursor(SDL_Renderer* renderer, AssetManager* assetManager, InputSystem* inputSystem, const std::string& assetId)
    : m_renderer(renderer), 
    m_assetManager(assetManager), 
    m_inputSystem(inputSystem), 
    m_texture(nullptr), m_position(0, 0) {
    m_texture = assetManager->GetTexture(assetId);
    if (!m_texture) {
        std::cout << "Failed to load cursor texture: " << assetId << std::endl;
    }
}

Cursor::~Cursor() {
}

void Cursor::Update() {
    m_inputSystem->GetMouseState(m_position);
}

void Cursor::Render() {
    if (!m_texture) return;
    
    SDL_Rect destRect = { static_cast<int>(m_position.x), static_cast<int>(m_position.y), 128, 128 };
    SDL_RenderCopy(m_renderer, m_texture, nullptr, &destRect);
}
