#include <states/HighscoreState.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <utils/GameSettings.h>
#include <ui/Button.h>
#include <functional>

HighscoreState::HighscoreState(Renderer* renderer, AssetManager* assetManager, InputSystem* input, SoundManager* soundManager)
  : State(renderer, assetManager, input, soundManager), 
    m_uiManager(nullptr),
    m_saveSettings("data/highscores.dat"),
    m_displayCount(5) {
}

HighscoreState::~HighscoreState() {
}

void HighscoreState::Init() {
  std::cout << "HighscoreState initialized\n";
  
  InitializeBackground("background", 
                      GameSettings::SCREEN_WIDTH, 
                      GameSettings::SCREEN_HEIGHT, 
                      GameSettings::BACKGROUND_SCROLL_SPEED);
  InitializeCursor("mouse_cursor");
  
  m_topScores = m_saveSettings.GetTopScores(m_displayCount);
  SetupUI();
}

void HighscoreState::SetupUI() {
  m_uiManager = std::make_unique<UIManager>(m_renderer, m_assetManager, m_input);
  
  // Create title
  auto titleText = std::make_shared<Button>(
    m_renderer,
    m_assetManager,
    m_input,
    "highscore_title",
    [this]() { /* No action for title */ }
  );
  
  // Create back button
  auto backButton = std::make_shared<Button>(
    m_renderer, 
    m_assetManager, 
    m_input, 
    "button_back", 
    [this]() {
      RequestStateChange(GameStates::MENU);
    }
  );
  
  // Create reset button
  auto resetButton = std::make_shared<Button>(
    m_renderer, 
    m_assetManager, 
    m_input, 
    "button_reset", 
    [this]() {
      if (m_saveSettings.ResetHighScores()) {
        std::cout << "High scores reset successfully.\n";
        m_topScores.clear();
      }
    }
  );
  
  // Position UI elements
  int screenWidth = GameSettings::SCREEN_WIDTH;
  int screenHeight = GameSettings::SCREEN_HEIGHT;
  
  // Create textures using GetTextTexture
  SDL_Texture* titleTexture = m_assetManager->GetTexture("highscore_title");
  
  SDL_Texture* backTexture = m_assetManager->GetTextTexture(
    "button_back",
    "BACK TO MENU",
    {255, 255, 255, 255},
    "game_font",
    24
  );
  
  SDL_Texture* resetTexture = m_assetManager->GetTextTexture(
    "button_reset",
    "RESET SCORES",
    {255, 100, 100, 255},
    "game_font",
    24
  );
  
  int titleWidth, titleHeight;
  SDL_QueryTexture(titleTexture, NULL, NULL, &titleWidth, &titleHeight);
  titleText->SetPosition(screenWidth/2 - titleWidth/2, 80);
  
  int backButtonWidth, backButtonHeight;
  SDL_QueryTexture(backTexture, NULL, NULL, &backButtonWidth, &backButtonHeight);
  backButton->SetPosition(screenWidth/2 - backButtonWidth/2, screenHeight - 100);
  
  int resetButtonWidth, resetButtonHeight;
  SDL_QueryTexture(resetTexture, NULL, NULL, &resetButtonWidth, &resetButtonHeight);
  resetButton->SetPosition(screenWidth/2 - resetButtonWidth/2, screenHeight - 150);
  
  // Set sound effects for buttons
  backButton->SetHoverSound("button_hover");
  backButton->SetClickSound("button_click");
  resetButton->SetHoverSound("button_hover");
  resetButton->SetClickSound("button_click");
  
  // Add components to UI manager
  m_uiManager->AddComponent(titleText);
  m_uiManager->AddComponent(backButton);
  m_uiManager->AddComponent(resetButton);
}

void HighscoreState::HandleEvents() {
  m_uiManager->HandleEvents();
  
  if (m_input->IsKeyJustPressed(SDLK_ESCAPE)) {
    RequestStateChange(GameStates::MENU);
  }
  
  if (m_input->IsQuitRequested()) {
    RequestStateChange(GameStates::QUIT);
  }
}

void HighscoreState::Update() {
  m_cursor->Update();
  m_uiManager->Update();
}

void HighscoreState::Render() {
  m_backgroundManager->RenderBackground();
  m_backgroundManager->InfiniteBackground();
  
  m_uiManager->Render();
  DisplayHighScores();
  
  m_cursor->Render();
}

void HighscoreState::DisplayHighScores() {
  SDL_Renderer* sdlRenderer = m_renderer->GetSDLRenderer();
  
  m_topScores = m_saveSettings.GetTopScores(m_displayCount);
  
  int yOffset = 180;
  int index = 0;
  
  for (int score : m_topScores) {
    std::string scoreKey = "high_score_" + std::to_string(index);
    std::string scoreText = std::to_string(index + 1) + ". " + std::to_string(score);
    
    SDL_Texture* scoreTexture = m_assetManager->GetTextTexture(
      scoreKey,
      scoreText,
      {255, 255, 255, 255},
      "game_font",
      24
    );
    
    if (scoreTexture) {
      int textWidth, textHeight;
      SDL_QueryTexture(scoreTexture, NULL, NULL, &textWidth, &textHeight);
      
      SDL_Rect textRect = {
        (GameSettings::SCREEN_WIDTH - textWidth) / 2,
        yOffset,
        textWidth,
        textHeight
      };
      
      SDL_RenderCopy(sdlRenderer, scoreTexture, NULL, &textRect);
      
      yOffset += textHeight + 15;
      index++;
    }
  }
  
  if (m_topScores.empty()) {
    SDL_Texture* noScoresTexture = m_assetManager->GetTextTexture(
      "no_scores_text",
      "No scores yet. Play a game!",
      {180, 180, 180, 255},
      "game_font",
      24
    );
    
    if (noScoresTexture) {
      int textWidth, textHeight;
      SDL_QueryTexture(noScoresTexture, NULL, NULL, &textWidth, &textHeight);
      
      SDL_Rect textRect = {
        (GameSettings::SCREEN_WIDTH - textWidth) / 2,
        yOffset,
        textWidth,
        textHeight
      };
      
      SDL_RenderCopy(sdlRenderer, noScoresTexture, NULL, &textRect);
    }
  }
}

void HighscoreState::Cleanup() {
  std::cout << "HighscoreState cleaned up\n";
}