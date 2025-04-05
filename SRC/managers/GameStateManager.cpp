#include <managers/GameStateManager.h>

void GameStateManager::RegisterState(GameStates state, std::shared_ptr<State> statePtr) {
    m_registeredStates[state] = statePtr;
    // Set the manager reference in the state
    statePtr->SetGameStateManager(this);
}

void GameStateManager::SwitchState(GameStates newState) {
    auto oldPtr = m_registeredStates[m_currentState];
    if (oldPtr) {
        oldPtr->Cleanup();
    }
    m_currentState = newState;
    auto newPtr = m_registeredStates[m_currentState];
    if (newPtr) {
        newPtr->Init();
    }
}

GameStates GameStateManager::GetCurrentState() const {
    return m_currentState;
}

void GameStateManager::HandleEvents() {
    auto currentPtr = m_registeredStates[m_currentState];
    if (currentPtr) {
        currentPtr->HandleEvents();
    }
}

void GameStateManager::Update() {
    auto currentPtr = m_registeredStates[m_currentState];
    if (currentPtr) {
        currentPtr->Update();
    }
}

void GameStateManager::Render() {
    auto currentPtr = m_registeredStates[m_currentState];
    if (currentPtr) {
        currentPtr->Render();
    }
}
