#pragma once

#include <states/State.h>
#include <entities/enemy/EnemySpawner.h>
#include <entities/enemy/Enemy.h>
#include <entities/player/Player.h>
#include <utils/Timer.h>

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

    EnemySpawner* m_enemySpawner;
    Player* m_player;
    
    Timer m_gameTimer;
    float m_deltaTime;
};
