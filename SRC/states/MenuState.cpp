#include <states/MenuState.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <managers/GameStateManager.h> 
#include <ui/Button.h>
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
    
    // Create buttons
    auto playButton = std::make_shared<Button>(
        m_renderer, 
        m_assetManager, 
        m_input, 
        "button_play", 
        [this]() {
            RequestStateChange(GameStates::PLAYING);
        }
    );
    
    auto optionsButton = std::make_shared<Button>(
        m_renderer, 
        m_assetManager, 
        m_input, 
        "button_options", 
        [this]() {
            // TODO: Implement options state
            std::cout << "Options button clicked\n";
        }
    );
    
    auto quitButton = std::make_shared<Button>(
        m_renderer, 
        m_assetManager, 
        m_input, 
        "button_quit", 
        [this]() {
            RequestStateChange(GameStates::QUIT);
        }
    );
    
    // Position buttons vertically centered with spacing
    int screenWidth = 800;
    int screenHeight = 800;
    int buttonHeight = playButton->GetRect().h;
    int spacing = 20;
    
    // Calculate starting Y position to center the group
    int totalHeight = buttonHeight * 3 + spacing * 2;
    int startY = (screenHeight - totalHeight) / 2;
    int offSetX = 100;
    int offSetY = 100;

    // Position each button
    playButton->SetPosition(screenWidth/2 - playButton->GetRect().w/2 - offSetX, startY + offSetY);
    optionsButton->SetPosition(screenWidth/2 - optionsButton->GetRect().w/2 - offSetX, startY + buttonHeight + spacing + offSetY);
    quitButton->SetPosition(screenWidth/2 - quitButton->GetRect().w/2 - offSetX, startY + (buttonHeight + spacing) * 2 + offSetY);
    
    // Add buttons to UI manager
    m_uiManager->AddComponent(playButton);
    m_uiManager->AddComponent(optionsButton);
    m_uiManager->AddComponent(quitButton);
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
