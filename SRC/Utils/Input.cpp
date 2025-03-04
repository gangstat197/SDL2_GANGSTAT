#include <Utils/Input.h>
#include <Utils/Vector2D.h>

Input::Input()
{
    // Initialize member variables
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        m_keys[i] = false;
        m_prevKeys[i] = false;
    }
    m_mouseButtons = 0;
    m_prevMouseButtons = 0;
    m_mouseX = 0;
    m_mouseY = 0;
}

Input::~Input() {
}

void Input::handleEvents()
{
    // Handle SDL events
    while (SDL_PollEvent(&m_event))
    {
        switch (m_event.type)
        {
            case SDL_KEYDOWN:
                m_keys[m_event.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                m_keys[m_event.key.keysym.scancode] = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_mouseButtons |= SDL_BUTTON(m_event.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_mouseButtons &= ~SDL_BUTTON(m_event.button.button);
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

bool Input::isKeyPressed(SDL_Keycode key)
{
    Uint8 scancode = SDL_GetScancodeFromKey(key);
    return m_keys[scancode];
}

bool Input::isKeyReleased(SDL_Keycode key)
{
    Uint8 scancode = SDL_GetScancodeFromKey(key);
    return !m_keys[scancode];
}

bool Input::isMouseButtonPressed(Uint8 button)
{
    return (m_mouseButtons & SDL_BUTTON(button)) != 0;
}

bool Input::isMouseButtonReleased(Uint8 button)
{
    return (m_mouseButtons & SDL_BUTTON(button)) == 0;
}

void Input::GetMouseState(Vector2D& mousePosition) {
    int x, y; 
    SDL_GetMouseState(&x, &y);
    mousePosition = Vector2D(x, y);
    mousePosition.print();
}
