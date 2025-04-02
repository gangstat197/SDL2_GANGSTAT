#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <utils/Vector2D.h>

class AssetManager;
class InputSystem;

class Cursor {
public:
    Cursor(SDL_Renderer* renderer, AssetManager* assetManager, InputSystem* inputSystem, const std::string& assetId);
    ~Cursor();

    void Update();
    void Render();

private:
    SDL_Renderer* m_renderer;
    AssetManager* m_assetManager;
    InputSystem* m_inputSystem;
    SDL_Texture* m_texture;
    
    Vector2D m_position; // Use Vector2D for position
};
