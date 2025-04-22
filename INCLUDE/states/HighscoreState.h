#pragma once

#include <states/State.h>
#include <systems/SaveSettings.h>
#include <ui/UIManager.h>
#include <memory>
#include <vector>

class HighscoreState : public State {
public:
    HighscoreState(Renderer* renderer, AssetManager* assetManager, InputSystem* input, SoundManager* soundManager);
    virtual ~HighscoreState();

    void Init() override;
    void HandleEvents() override;
    void Update() override;
    void Render() override;
    void Cleanup() override;

private:
    void SetupUI();
    void DisplayHighScores();
    
    std::unique_ptr<UIManager> m_uiManager;
    SaveSettings m_saveSettings;
    std::vector<int> m_topScores;
    int m_displayCount;
};