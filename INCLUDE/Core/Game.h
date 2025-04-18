#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include <core/Renderer.h>

#include <systems/InputSystem.h>
#include <managers/AssetManager.h>
#include <managers/GameStateManager.h>
#include <managers/SoundManager.h>

#include <states/MenuState.h>
#include <states/PlayingState.h>
#include <states/QuitState.h>

class Game {
public:
    Game();
    ~Game();

    bool Init(const char* title, int width, int height);

    void LoadAssets();
    void InitStates();

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
    GameStateManager* stateManager;
    SoundManager* soundManager;

    // States
    std::shared_ptr<MenuState> menuState;
    std::shared_ptr<PlayingState> playingState;
    std::shared_ptr<QuitState> quitState;

    bool isRunning;
    int frameCount;
};

#endif // GAME_H
