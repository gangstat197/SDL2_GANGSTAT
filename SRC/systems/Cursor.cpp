#include <systems\Cursor.h>
#include <managers\AssetManager.h>
#include <systems\InputSystem.h>
#include <utils\Vector2D.h>
#include <SDL2/SDL.h>

Cursor::Cursor(SDL_Renderer* renderer, AssetManager* assetManager, InputSystem* inputSystem, const std::string& assetId)
    : m_renderer(renderer), 
    m_assetManager(assetManager), 
    m_inputSystem(inputSystem), 
    m_texture(nullptr), m_position(0, 0) {
    m_texture = m_assetManager->LoadTexture(assetId, "assets/images/cursor_yellow.png", m_renderer);
}

Cursor::~Cursor() {
}

void Cursor::Update() {
    // Get mouse position
    m_inputSystem->GetMouseState(m_position); // if you have a Vector2D overload, adjust accordingly
}

void Cursor::Render() {
    if (!m_texture) return;
    SDL_Rect destRect = { m_position.x, m_position.y, 128, 128 };
    SDL_RenderCopy(m_renderer, m_texture, nullptr, &destRect);
}
