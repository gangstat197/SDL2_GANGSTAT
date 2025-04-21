#pragma once

#include <SDL2/SDL.h>

class Timer {
public:
    Timer() : m_startTicks(0), m_pausedTicks(0), m_paused(false), m_started(false) {}

    void Start();
    void Stop();
    void Pause();
    void Resume();
    Uint32 GetTicks() const;
    float GetSeconds() const;
    bool IsStarted() const;
    bool IsPaused() const;

private:
    Uint32 m_startTicks;
    Uint32 m_pausedTicks;
    bool m_paused;
    bool m_started;
}; 