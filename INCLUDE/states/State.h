#pragma once
#include <core/Renderer.h>
#include <managers/AssetManager.h>
#include <managers/BackgroundManager.h>
#include <systems/Cursor.h>
#include <systems/InputSystem.h>

// Forward declare GameStateManager to avoid circular inclusion
class GameStateManager;

// Define game states at this level so all states can access them
enum class GameStates {
    MENU,
    PLAYING,
    WIN,
    DEAD,
    QUIT
};

class State {
public:
    // Constructor with common dependencies
    State(Renderer* renderer, AssetManager* assetManager, InputSystem* input);
    virtual ~State();
    
    // Set the GameStateManager reference - called by the manager during registration
    void SetGameStateManager(GameStateManager* manager);
    
    // Core state methods
    virtual void Init() = 0;
    virtual void HandleEvents() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Cleanup() = 0;
    
protected:
    // Request a state change
    void RequestStateChange(GameStates newState);
    
    // Common resources 
    GameStateManager* m_stateManager;
    Renderer* m_renderer;
    AssetManager* m_assetManager;
    InputSystem* m_input;
    
    // Optional components 
    BackgroundManager* m_backgroundManager;
    Cursor* m_cursor;
    
    // Common state initialization
    void InitializeBackground(const char* textureKey, int width, int height, int scrollSpeed = 0);
    void InitializeCursor(const char* textureKey);
    void CleanupCommonResources();
};
