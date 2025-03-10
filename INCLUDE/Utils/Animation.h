#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <SDL.h>
#include "Utils/Timer.h"
#include "Utils/SpriteSheet.h"

class Animation {
public:
    Animation();
    ~Animation();

    void SetSpriteSheet(SpriteSheet* spriteSheet);
    void SetFrameRate(int frameRate);
    void Update();
    void Render(SDL_Renderer* renderer, int x, int y);

private:
    SpriteSheet* m_spriteSheet;
    int m_frameRate;
    int m_currentFrame;
    Timer m_timer;
};

#endif // ANIMATION_H
