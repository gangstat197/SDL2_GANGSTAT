#pragma once

#include <states/State.h>
#include <entities/EnemySpawner.h>
#include <entities/Enemy.h>

class PlayingState : public State {
public:
    PlayingState(Renderer* renderer, AssetManager* assetManager, InputSystem* input);
    ~PlayingState();

    void Init() override;
    void HandleEvents() override;
    void Update() override;
    void Render() override;
    void Cleanup() override;

private: 
    void InitializeEnemySpawner();

    EnemySpawner* m_enemySpawner;
    Enemy* m_test_enemy;
};
