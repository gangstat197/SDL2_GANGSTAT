#include "Utils/Animation.h"

Animation::Animation() : m_spriteSheet(nullptr), m_frameRate(0), m_currentFrame(0) {}

Animation::~Animation() {}

void Animation::SetSpriteSheet(SpriteSheet* spriteSheet) {
    m_spriteSheet = spriteSheet;
}

void Animation::SetFrameRate(int frameRate) {
    m_frameRate = frameRate;
    m_timer.Start();
}

void Animation::Update() {
    if (m_timer.GetTicks() > (1000 / m_frameRate)) {
        m_currentFrame = (m_currentFrame + 1) % m_spriteSheet->GetClips().size();
        m_timer.Start(); // Restart the timer for the next frame
    }
}

void Animation::Render(SDL_Renderer* renderer, int x, int y) {
    if (m_spriteSheet) {
        m_spriteSheet->Render(x, y, &m_spriteSheet->GetClips()[m_currentFrame]);
    }
}
