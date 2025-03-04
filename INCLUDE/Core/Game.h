#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include <Entities/BaseUnit.h>
#include <Core/Renderer.h>
#include <Utils/Input.h>

class Game {
public:
    Game();
    ~Game();

    bool Init(const char* title, int width, int height);
    void HandleEvents();
    void Update();
    void Render();
    void Clean();
    bool IsRunning() const;

private:
    Renderer* renderer; 
    Input* input;
    SDL_Texture* backgroundTexture;
    bool isRunning;

    std::vector<BaseUnit*> entities;
};

#endif // GAME_H
