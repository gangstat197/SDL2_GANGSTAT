#pragma once

#include <states/State.h>
#include <core/Renderer.h>
#include <managers/AssetManager.h>
#include <systems/InputSystem.h>
#include <ui/UIManager.h>
#include <memory>

class MenuState : public State {
public:
  MenuState(Renderer* renderer, AssetManager* assetManager, InputSystem* input);
  ~MenuState();
  
  void Init() override;
  void HandleEvents() override;
  void Update() override;
  void Render() override;
  void Cleanup() override;

private:
  std::unique_ptr<UIManager> m_uiManager;
};
