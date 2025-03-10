#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class Timer {
public:
    Timer();
    ~Timer();

    void Start();
    void Stop();
    void Pause();
    void Unpause();

    Uint32 GetTicks();
    bool IsStarted();
    bool IsPaused();

private:
    Uint32 m_startTicks;
    Uint32 m_pausedTicks;
    bool m_paused;
    bool m_started;
};

#endif // TIMER_H