#pragma once
#include <core/Renderer.h>
#include <managers/AssetManager.h>
#include <managers/BackgroundManager.h>
#include <systems/Cursor.h>
#include <systems/InputSystem.h>
#include <managers/SoundManager.h>

class GameStateManager;

enum class GameStates {
    MENU,
    PLAYING,
    WIN,
    DEAD,
    QUIT
};

class State {
public:
    State(Renderer* renderer, AssetManager* assetManager, InputSystem* input, SoundManager* soundManager);
    virtual ~State();
    
    void SetGameStateManager(GameStateManager* manager);
    
    virtual void Init() = 0;
    virtual void HandleEvents() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Cleanup() = 0;
    
protected:
    void RequestStateChange(GameStates newState);
    
    GameStateManager* m_stateManager;
    Renderer* m_renderer;
    AssetManager* m_assetManager;
    InputSystem* m_input;
    SoundManager* m_soundManager;

    BackgroundManager* m_backgroundManager;
    Cursor* m_cursor;
    
    void InitializeBackground(const char* textureKey, int width, int height, int scrollSpeed = 0);
    void InitializeCursor(const char* textureKey);
    void CleanupCommonResources();
};
