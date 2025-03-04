#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <Utils/Vector2D.h>

class Input
{
public:
    Input();
    ~Input();

    void handleEvents();

    bool isKeyPressed(SDL_Keycode key);
    bool isKeyReleased(SDL_Keycode key);
    bool isMouseButtonPressed(Uint8 button);
    bool isMouseButtonReleased(Uint8 button);
    
    void GetMouseState(Vector2D& mousePosition);
    

private:
    SDL_Event m_event;
    bool m_keys[SDL_NUM_SCANCODES];
    bool m_prevKeys[SDL_NUM_SCANCODES];
    Uint32 m_mouseButtons;
    Uint32 m_prevMouseButtons;
    int m_mouseX;
    int m_mouseY;
};

#endif // INPUT_H
