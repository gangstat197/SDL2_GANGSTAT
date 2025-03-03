#ifndef GAME_H
#define GAME_H

#include <SDL.h>

class Renderer; // Forward declaration

class Game {
public:
    Game();
    ~Game();

    bool Init(const char* title, int width, int height);
    void HandleEvents();
    void Update();
    void Render();
    void Clean();
    bool IsRunning() const;

private:
    SDL_Window* window;
    Renderer* renderer; // Raw pointer
    SDL_Texture* backgroundTexture;
    bool isRunning;
};

#endif // GAME_H
