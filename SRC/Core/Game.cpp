#include <core/Game.h>
#include <core/Renderer.h>

#include <systems/InputSystem.h>
#include <utils/Vector2D.h>
#include <utils/SpriteSheet.h>
#include <utils/GameSettings.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>

Game::Game() : 
    input(nullptr), 
    renderer(nullptr), 
    isRunning(false), 
    assetManager(nullptr),
    stateManager(nullptr),
    soundManager(nullptr) {}

Game::~Game() {
    Clean();
}

bool Game::Init(const char* title, int width, int height) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    // Core components
    renderer = new Renderer(title, width, height);
    input = new InputSystem();
    assetManager = &AssetManager::Instance();
    soundManager = &SoundManager::Instance();

    // Initialize sound manager
    if (!soundManager->Initialize()) {
        std::cerr << "Failed to initialize sound manager" << std::endl;
        return false;
    }

    LoadAssets();

    // State manager
    
    stateManager = new GameStateManager();
    InitStates();
    
    stateManager->SwitchState(GameStates::MENU);

    frameCount = 0;
    
    isRunning = true;
    return true;
}

void Game::LoadAssets() {
    // Load Background texture
    assetManager->LoadTexture("background", "assets/images/WhiteGrid_Background.png", renderer->GetSDLRenderer());

    // Load Heading texture with scaling
    SDL_Texture* headingTexture = assetManager->LoadTexture("heading", "assets/images/MicroDash_Hero.png", renderer->GetSDLRenderer(), 0.25);
    if (!headingTexture) {
        std::cout << "Failed to load heading texture\n";
        return;
    }
    
    // Load cursor texture
    assetManager->LoadTexture("mouse_cursor", "assets/images/cursor_yellow.png", renderer->GetSDLRenderer());
    
    // Load UI textures with scaling
    assetManager->LoadTexture("button_play", "assets/images/ui/btn_play.png", renderer->GetSDLRenderer(), 0.25);
    assetManager->LoadTexture("button_options", "assets/images/ui/btn_options.png", renderer->GetSDLRenderer(), 0.25);
    assetManager->LoadTexture("button_quit", "assets/images/ui/btn_quit.png", renderer->GetSDLRenderer(), 0.25);
    assetManager->LoadTexture("debug_position", "assets/images/debug/debug_reddot.png", renderer->GetSDLRenderer(), 0.5);

    // Load player texture with scaling
    assetManager->LoadTexture("player", "assets/images/player/mr_square.png", renderer->GetSDLRenderer(), 0.15);

    // Load hit effect
    assetManager->LoadTexture("hit_vignette", "assets/images/player/hit_vignette.png", renderer->GetSDLRenderer());
    
    // Load enemy textures
    assetManager->LoadTexture("enemy_round", "assets/images/enemy/bacteria_round.png", renderer->GetSDLRenderer(), 1);
    assetManager->LoadTexture("enemy_round01", "assets/images/enemy/bacteria_round01.png", renderer->GetSDLRenderer(), 0.8);
    assetManager->LoadTexture("enemy_round02", "assets/images/enemy/bacteria_round02.png", renderer->GetSDLRenderer(), 0.6);
    assetManager->LoadTexture("enemy_rectangle", "assets/images/enemy/bacteria_rectangle.png", renderer->GetSDLRenderer(), 0.8);
    assetManager->LoadTexture("enemy_polyon01", "assets/images/enemy/bacteria_polygon01.png", renderer->GetSDLRenderer(), 0.8);
    assetManager->LoadTexture("enemy_polyon02", "assets/images/enemy/bacteria_polygon02.png", renderer->GetSDLRenderer(), 0.5);
    assetManager->LoadTexture("enemy_polyon03", "assets/images/enemy/bacteria_polygon03.png", renderer->GetSDLRenderer(), 0.5);
    
    // Load music
    assetManager->LoadMusic("main_theme", "assets/sounds/main_theme.mp3");

    // Load sound effects
    assetManager->LoadSound("button_click", "assets/sounds/button_click.wav");
    assetManager->LoadSound("button_hover", "assets/sounds/button_hover.wav");
    assetManager->LoadSound("player_hit", "assets/sounds/hit.wav");

    // Set Sound Manager Volume and state based on settings
    soundManager->SetMusicVolume(static_cast<int>(GameSettings::MUSIC_VOLUME * MIX_MAX_VOLUME));  
    soundManager->SetSoundVolume(static_cast<int>(GameSettings::SOUND_VOLUME * MIX_MAX_VOLUME));
    soundManager->SetMusicEnabled(GameSettings::MUSIC_ENABLED);
    soundManager->SetSoundEnabled(GameSettings::SOUND_ENABLED);
    
    if (GameSettings::MUSIC_ENABLED) {
        soundManager->PlayMusic("main_theme", -1);
    }
}

void Game::InitStates() {
    // Initialize states 
    menuState = std::make_shared<MenuState>(renderer, assetManager, input, soundManager);
    playingState = std::make_shared<PlayingState>(renderer, assetManager, input, soundManager);
    quitState = std::make_shared<QuitState>(renderer, assetManager, input, soundManager, isRunning);
    
    // Register states 
    stateManager->RegisterState(GameStates::MENU, menuState);
    stateManager->RegisterState(GameStates::PLAYING, playingState);
    stateManager->RegisterState(GameStates::QUIT, quitState);
}
    
void Game::HandleEvents() {
    input->HandleEvents();

    stateManager->HandleEvents();

    if (input->IsQuitRequested()) {
        stateManager->SwitchState(GameStates::QUIT);
    }

    if (input->IsKeyJustPressed(SDLK_ESCAPE)) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
    
    if (input->IsMouseButtonJustPressed(SDL_BUTTON_LEFT)) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
}

void Game::Render() {
    renderer->Clear();
    
    stateManager->Render();
    
    renderer->Present();
}

void Game::Update() {
    stateManager->Update();


}

void Game::Clean() {
    assetManager->UnloadAll(); 

    if (stateManager) {
        delete stateManager;
        stateManager = nullptr;
    }

    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }

    if (input) {
        delete input;
        input = nullptr;
    }
    
    SDL_Quit();
}


bool Game::IsRunning() {
    return isRunning;
}