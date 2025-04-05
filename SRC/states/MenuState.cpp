#include <states/MenuState.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <managers/GameStateManager.h> // Include for GameStates enum
#include <ui/Button.h>
#include <ui/Panel.h>
#include <ui/Label.h>
#include <functional>

MenuState::MenuState(Renderer* renderer, AssetManager* assetManager, InputSystem* input)
    : State(renderer, assetManager, input), m_uiManager(nullptr) {
}

MenuState::~MenuState() {
}

void MenuState::Init() {
    std::cout << "MenuState initialized\n";
    
    // Background & Cursor
    InitializeBackground("background", 800, 800, 100);
    InitializeCursor("mouse_cursor");
    
    // UI manager
    m_uiManager = std::make_unique<UIManager>(m_renderer, m_assetManager, m_input);
    
    // Play Button
    auto playButton = std::make_shared<Button>(
        m_renderer, 
        m_assetManager, 
        m_input, 
        "button_play", 
        [this]() {
            RequestStateChange(GameStates::PLAYING);
        }
    );
    
    playButton->SetPosition(250, 400);
    
    m_uiManager->AddComponent(playButton);
}

void MenuState::HandleEvents() {
    m_uiManager->HandleEvents();
    
    // Quit 
    if (m_input->IsQuitRequested()) {
        RequestStateChange(GameStates::QUIT);
    }
}

void MenuState::Update() {
    m_cursor->Update();
    m_uiManager->Update();
}

void MenuState::Render() {
    // Background
    m_backgroundManager->RenderBackground();
    m_backgroundManager->InfiniteBackground();
    
    // Title
    m_renderer->RenderTexture(m_assetManager->GetTexture("heading"), 100, 120);
    
    // UI
    m_uiManager->Render();
    
    // Cursor
    m_cursor->Render();
}

void MenuState::Cleanup() {
    std::cout << "MenuState cleaned up\n";
}
