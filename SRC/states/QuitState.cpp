#include <states/QuitState.h>
#include <iostream>

QuitState::QuitState(Renderer* renderer, AssetManager* assetManager, InputSystem* input, bool &isRunning)
    : State(renderer, assetManager, input) {
    m_isRunning = &isRunning;
}

QuitState::~QuitState() {
}

void QuitState::Init() {
    std::cout << "QuitState initialized\n";
}

void QuitState::HandleEvents() {
}

void QuitState::Update() {
    *m_isRunning = false;
}

void QuitState::Render() {
}

void QuitState::Cleanup() {
    std::cout << "QuitState cleaned up\n";
}

