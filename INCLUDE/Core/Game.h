#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "../core/Renderer.h"
#include <systems/InputSystem.h>

class Game {
public:
    Game();
    ~Game();

    bool Init(const char* title, int width, int height);
    void HandleEvents();
    void Update();
    void Render();
    void Clean();
    bool IsRunning();

private:
    Renderer* renderer; 
    InputSystem* input;
    SDL_Texture* backgroundTexture;
    bool isRunning;
    int frameCount;
};

#endif // GAME_H
