#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <unordered_map>
#include "utils/Vector2D.h"

class InputSystem {
public:
    InputSystem();
    ~InputSystem();

    void HandleEvents();

    bool IsKeyPressed(SDL_Keycode key);
    bool IsKeyReleased(SDL_Keycode key);
    bool IsKeyJustPressed(SDL_Keycode key);
    bool IsKeyJustReleased(SDL_Keycode key);

    bool IsMouseButtonPressed(Uint8 button);
    bool IsMouseButtonReleased(Uint8 button);
    bool IsMouseButtonJustPressed(Uint8 button);
    bool IsMouseButtonJustReleased(Uint8 button);

    bool IsQuitRequested();

    void GetMouseState(Vector2D& mousePosition);

private:
    SDL_Event m_event;
    std::unordered_map<SDL_Scancode, bool> m_currentKeys;
    std::unordered_map<SDL_Scancode, bool> m_previousKeys;
    Uint32 m_currentMouseButtons;
    Uint32 m_previousMouseButtons;
    int m_mouseX;
    int m_mouseY;

    bool m_quitRequested; 

    void UpdateKeyState(SDL_Scancode scancode, bool isPressed);
    void UpdateMouseButtonState(Uint8 button, bool isPressed);
};

#endif // INPUT_H