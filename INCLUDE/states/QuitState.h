#pragma once

#include <states/State.h>

class QuitState : public State {
public:
    QuitState(Renderer* renderer, AssetManager* assetManager, InputSystem* input, bool &isRunning);
    ~QuitState();

    void Init() override;
    void HandleEvents() override;
    void Update() override;
    void Render() override;
    void Cleanup() override;

protected:
    bool* m_isRunning;
};
