#include <Utils/Input.h>
#include <Utils/Vector2D.h>

Input::Input() : m_currentMouseButtons(0), m_previousMouseButtons(0), m_mouseX(0), m_mouseY(0) {}

Input::~Input() {}

void Input::HandleEvents() {
    m_previousKeys = m_currentKeys;
    m_previousMouseButtons = m_currentMouseButtons;

    while (SDL_PollEvent(&m_event)) {
        switch (m_event.type) {
        case SDL_KEYDOWN:
            UpdateKeyState(m_event.key.keysym.scancode, true);
            break;
        case SDL_KEYUP:
            UpdateKeyState(m_event.key.keysym.scancode, false);
            break;
        case SDL_MOUSEBUTTONDOWN:
            UpdateMouseButtonState(m_event.button.button, true);
            break;
        case SDL_MOUSEBUTTONUP:
            UpdateMouseButtonState(m_event.button.button, false);
            break;
        case SDL_MOUSEMOTION:
            m_mouseX = m_event.motion.x;
            m_mouseY = m_event.motion.y;
            break;
        default:
            break;
        }
    }
}

void Input::UpdateKeyState(SDL_Scancode scancode, bool isPressed) {
    m_currentKeys[scancode] = isPressed;
}

void Input::UpdateMouseButtonState(Uint8 button, bool isPressed) {
    if (isPressed) {
        m_currentMouseButtons |= SDL_BUTTON(button);
    }
    else {
        m_currentMouseButtons &= ~SDL_BUTTON(button);
    }
}

bool Input::IsKeyPressed(SDL_Keycode key) {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    return m_currentKeys[scancode];
}

bool Input::IsKeyReleased(SDL_Keycode key) {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    return !m_currentKeys[scancode];
}

bool Input::IsKeyJustPressed(SDL_Keycode key) {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    return m_currentKeys[scancode] && !m_previousKeys[scancode];
}

bool Input::IsKeyJustReleased(SDL_Keycode key) {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    return !m_currentKeys[scancode] && m_previousKeys[scancode];
}

bool Input::IsMouseButtonPressed(Uint8 button) {
    return (m_currentMouseButtons & SDL_BUTTON(button)) != 0;
}

bool Input::IsMouseButtonReleased(Uint8 button) {
    return (m_currentMouseButtons & SDL_BUTTON(button)) == 0;
}

bool Input::IsMouseButtonJustPressed(Uint8 button) {
    return (m_currentMouseButtons & SDL_BUTTON(button)) != 0 && (m_previousMouseButtons & SDL_BUTTON(button)) == 0;
}

bool Input::IsMouseButtonJustReleased(Uint8 button) {
    return (m_currentMouseButtons & SDL_BUTTON(button)) == 0 && (m_previousMouseButtons & SDL_BUTTON(button)) != 0;
}

void Input::GetMouseState(Vector2D& mousePosition) {
    SDL_GetMouseState(&m_mouseX, &m_mouseY);
    mousePosition = Vector2D(m_mouseX, m_mouseY);
}
