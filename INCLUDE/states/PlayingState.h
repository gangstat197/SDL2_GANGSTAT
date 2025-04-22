#pragma once

#include <states/State.h>
#include <entities/enemy/EnemySpawner.h>
#include <entities/enemy/Enemy.h>
#include <entities/player/Player.h>
#include <entities/powerup/PowerUpSpawner.h>
#include <utils/Timer.h>
#include <systems/SaveSettings.h>

enum class PlayState {
    PLAYING,
    GAME_OVER
};

class PlayingState : public State {
public:
    PlayingState(Renderer* renderer, AssetManager* assetManager, InputSystem* input, SoundManager* soundManager);
    ~PlayingState();

    void Init() override;
    void HandleEvents() override;
    void Update() override;
    void Render() override;
    void Cleanup() override;

private: 
    void InitPlayer();
    void InitEnemySpawner();
    void InitPowerUpSpawner();
    void CheckGameOver();
    void RenderGameOver();
    void Reset();

    void RenderScore();
    void SaveHighScore();

    EnemySpawner* m_enemySpawner;
    PowerUpSpawner* m_powerUpSpawner;
    Player* m_player;
    
    PlayState m_playState;
    
    Timer m_gameTimer;
    Timer m_gameOverTimer;
    float m_deltaTime;
    float m_currentTime;
    int m_finalScore;
    SaveSettings m_saveSettings;
    bool m_scoreWasSaved;
};
