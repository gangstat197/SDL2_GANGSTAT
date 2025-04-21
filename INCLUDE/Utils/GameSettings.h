#pragma once

namespace GameSettings {
    // Window settings
    inline const char* GAME_TITLE = "Micro Dash";
    inline constexpr int SCREEN_WIDTH = 600;
    inline constexpr int SCREEN_HEIGHT = 800;
    
    // Audio settings
    inline constexpr bool MUSIC_ENABLED = true;
    inline constexpr bool SOUND_ENABLED = true;
    inline constexpr float MUSIC_VOLUME = 0.7f;  
    inline constexpr float SOUND_VOLUME = 0.8f;  
    
    // Gameplay settings
    inline constexpr float BACKGROUND_SCROLL_SPEED = 150.0f;
    inline constexpr float ENEMY_BASE_SPEED = 200.0f;
    inline constexpr float ENEMY_SPEED_INCREMENT = 10.0f;  
    inline constexpr int SPAWN_RATE_MS = 1600;  
    inline constexpr int DIFFICULTY_INCREASE_TIME_MS = 10000;  
    
    // Power-up settings
    inline constexpr float POWERUP_SPAWN_CHANCE = 0.1f;  
    inline constexpr int POWERUP_DURATION_MS = 5000;  
    
    // Scoring
    inline constexpr int SCORE_PER_SECOND = 10;  
} 