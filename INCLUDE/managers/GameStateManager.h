#pragma once

#include <map>
#include <memory>
#include <states/State.h>

class GameStateManager {
public:
    void SwitchState(GameStates newState);
    GameStates GetCurrentState() const;
    void RegisterState(GameStates state, std::shared_ptr<State> statePtr);
    void HandleEvents();
    void Update();
    void Render();

private:
    GameStates m_currentState = GameStates::MENU;
    std::map<GameStates, std::shared_ptr<State>> m_registeredStates;
};
