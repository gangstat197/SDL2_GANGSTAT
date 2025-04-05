#include <states/State.h>
#include <managers/GameStateManager.h>
#include <iostream>

// Base state implementation is empty since it's purely virtual
// Individual state implementations are in their own files now

State::State(Renderer* renderer, AssetManager* assetManager, InputSystem* input)
    : m_stateManager(nullptr),
      m_renderer(renderer),
      m_assetManager(assetManager),
      m_input(input),
      m_backgroundManager(nullptr),
      m_cursor(nullptr) {
}

State::~State() {
    CleanupCommonResources();
}

void State::SetGameStateManager(GameStateManager* manager) {
    m_stateManager = manager;
}

void State::RequestStateChange(GameStates newState) {
    if (m_stateManager) {
        m_stateManager->SwitchState(newState);
    }
}

void State::InitializeBackground(const char* textureKey, int width, int height, int scrollSpeed) {
    if (m_backgroundManager) {
        delete m_backgroundManager;
    }
    
    m_backgroundManager = new BackgroundManager(
        m_renderer->GetSDLRenderer(),
        m_assetManager->GetTexture(textureKey),
        width,
        height
    );
    
    if (scrollSpeed > 0) {
        m_backgroundManager->SetScrollSpeed(scrollSpeed);
    }
}

void State::InitializeCursor(const char* textureKey) {
    if (m_cursor) {
        delete m_cursor;
    }
    
    m_cursor = new Cursor(
        m_renderer->GetSDLRenderer(),
        m_assetManager,
        m_input,
        textureKey
    );
}

void State::CleanupCommonResources() {
    if (m_backgroundManager) {
        delete m_backgroundManager;
        m_backgroundManager = nullptr;
    }
    
    if (m_cursor) {
        delete m_cursor;
        m_cursor = nullptr;
    }
}
