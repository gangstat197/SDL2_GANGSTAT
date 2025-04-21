#include <utils/Timer.h>

void Timer::Start() {
    m_started = true;
    m_paused = false;
    m_startTicks = SDL_GetTicks();
    m_pausedTicks = 0;
}

void Timer::Stop() {
    m_started = false;
    m_paused = false;
    m_startTicks = 0;
    m_pausedTicks = 0;
}

void Timer::Pause() {
    if (m_started && !m_paused) {
        m_paused = true;
        m_pausedTicks = SDL_GetTicks() - m_startTicks;
        m_startTicks = 0;
    }
}

void Timer::Resume() {
    if (m_started && m_paused) {
        m_paused = false;
        m_startTicks = SDL_GetTicks() - m_pausedTicks;
        m_pausedTicks = 0;
    }
}

Uint32 Timer::GetTicks() const {
    Uint32 time = 0;
    if (m_started) {
        if (m_paused) {
            time = m_pausedTicks;
        } else {
            time = SDL_GetTicks() - m_startTicks;
        }
    }
    return time;
}

float Timer::GetSeconds() const {
    return GetTicks() / 1000.0f;
}

bool Timer::IsStarted() const { 
    return m_started; 
}

bool Timer::IsPaused() const { 
    return m_paused && m_started; 
}
