#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "../core/Renderer.h"
#include <systems/InputSystem.h>
#include <managers/AssetManager.h>
#include <managers/BackgroundManager.h>
#include <systems/Cursor.h>

class Game {
public:
    Game();
    ~Game();

    bool Init(const char* title, int width, int height);

    void LoadAssets();

    void HandleEvents();
    void Update();
    void Render();
    void Clean();
    bool IsRunning();

private:
    // System
    Renderer* renderer; 
    InputSystem* input;

    // Manager
    AssetManager* assetManager;
    BackgroundManager* backgroundManager;

    Cursor* cursor;

    bool isRunning;
    int frameCount;
};

#endif // GAME_H
