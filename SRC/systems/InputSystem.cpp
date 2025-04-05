#include <systems/InputSystem.h>

InputSystem::InputSystem() 
    : m_currentMouseButtons(0), 
      m_previousMouseButtons(0), 
      m_mouseX(0), 
      m_mouseY(0),
      m_quitRequested(false) {}

InputSystem::~InputSystem() {}

void InputSystem::HandleEvents() {
    m_previousKeys = m_currentKeys;
    m_previousMouseButtons = m_currentMouseButtons;
    
    // Update mouse position every frame regardless of events
    SDL_GetMouseState(&m_mouseX, &m_mouseY);

    while (SDL_PollEvent(&m_event)) {
        switch (m_event.type) {
        case SDL_QUIT:
            m_quitRequested = true;
            break;
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

void InputSystem::UpdateKeyState(SDL_Scancode scancode, bool isPressed) {
    m_currentKeys[scancode] = isPressed;
}

void InputSystem::UpdateMouseButtonState(Uint8 button, bool isPressed) {
    if (isPressed) {
        m_currentMouseButtons |= SDL_BUTTON(button);
    }
    else {
        m_currentMouseButtons &= ~SDL_BUTTON(button);
    }
}

bool InputSystem::IsKeyPressed(SDL_Keycode key) {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    return m_currentKeys[scancode];
}

bool InputSystem::IsKeyReleased(SDL_Keycode key) {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    return !m_currentKeys[scancode];
}

bool InputSystem::IsKeyJustPressed(SDL_Keycode key) {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    return m_currentKeys[scancode] && !m_previousKeys[scancode];
}

bool InputSystem::IsKeyJustReleased(SDL_Keycode key) {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    return !m_currentKeys[scancode] && m_previousKeys[scancode];
}

bool InputSystem::IsMouseButtonPressed(Uint8 button) {
    return (m_currentMouseButtons & SDL_BUTTON(button)) != 0;
}

bool InputSystem::IsMouseButtonReleased(Uint8 button) {
    return (m_currentMouseButtons & SDL_BUTTON(button)) == 0;
}

bool InputSystem::IsMouseButtonJustPressed(Uint8 button) {
    return (m_currentMouseButtons & SDL_BUTTON(button)) != 0 && (m_previousMouseButtons & SDL_BUTTON(button)) == 0;
}

bool InputSystem::IsMouseButtonJustReleased(Uint8 button) {
    return (m_currentMouseButtons & SDL_BUTTON(button)) == 0 && (m_previousMouseButtons & SDL_BUTTON(button)) != 0;
}

void InputSystem::GetMouseState(Vector2D& mousePosition) {
    // Simply use the already updated mouse position
    mousePosition = Vector2D(m_mouseX, m_mouseY);
}

bool InputSystem::IsQuitRequested() {
    return m_quitRequested;
}
