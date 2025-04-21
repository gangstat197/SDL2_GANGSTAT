#include <states/MenuState.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <managers/GameStateManager.h> 
#include <ui/Button.h>
#include <functional>
#include <utils/GameSettings.h>

MenuState::MenuState(Renderer* renderer, AssetManager* assetManager, InputSystem* input, SoundManager* soundManager)
    : State(renderer, assetManager, input, soundManager), m_uiManager(nullptr) {
}

MenuState::~MenuState() {
}

void MenuState::Init() {
    std::cout << "MenuState initialized\n";
    
    InitializeBackground("background", 
                        GameSettings::SCREEN_WIDTH, 
                        GameSettings::SCREEN_HEIGHT, 
                        GameSettings::BACKGROUND_SCROLL_SPEED);
    InitializeCursor("mouse_cursor");
    
    m_uiManager = std::make_unique<UIManager>(m_renderer, m_assetManager, m_input);
    
    auto headingButton = std::make_shared<Button>(
        m_renderer,
        m_assetManager,
        m_input,
        "heading",
        [this]() { 
            std::cout << "Heading button clicked\n";
        }
    );
    
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
    
    int screenWidth = GameSettings::SCREEN_WIDTH;
    int screenHeight = GameSettings::SCREEN_HEIGHT;
    int buttonHeight = playButton->GetRect().h;
    int spacing = 20;
    
    int totalHeight = buttonHeight * 3 + spacing * 2;               
    int startY = (screenHeight - totalHeight) / 2;
    int offSetX = 100;
    int offSetY = 100;

    int headingWidth = m_assetManager->GetTextureWidth("heading");
    headingButton->SetPosition(screenWidth/2 - headingWidth/2, 120);
    
    playButton->SetPosition(screenWidth/2 - playButton->GetRect().w/2, startY + offSetY);
    optionsButton->SetPosition(screenWidth/2 - optionsButton->GetRect().w/2, startY + buttonHeight + spacing + offSetY);
    quitButton->SetPosition(screenWidth/2 - quitButton->GetRect().w/2, startY + (buttonHeight + spacing) * 2 + offSetY);
    
    headingButton->SetHoverSound("button_hover");
    
    playButton->SetHoverSound("button_hover");
    optionsButton->SetHoverSound("button_hover");
    quitButton->SetHoverSound("button_hover");

    playButton->SetClickSound("button_click");
    optionsButton->SetClickSound("button_click");
    quitButton->SetClickSound("button_click");

    m_uiManager->AddComponent(headingButton);
    m_uiManager->AddComponent(playButton);
    m_uiManager->AddComponent(optionsButton);
    m_uiManager->AddComponent(quitButton);
}

void MenuState::HandleEvents() {
    m_uiManager->HandleEvents();
    
    if (m_input->IsQuitRequested()) {
        RequestStateChange(GameStates::QUIT);
    }
}

void MenuState::Update() {
    m_cursor->Update();
    m_uiManager->Update();
}

void MenuState::Render() {
    m_backgroundManager->RenderBackground();
    m_backgroundManager->InfiniteBackground();
    
    m_uiManager->Render();
    
    m_cursor->Render();
}

void MenuState::Cleanup() {
    std::cout << "MenuState cleaned up\n";
}
