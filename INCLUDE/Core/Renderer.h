#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <string>
#include <Utils/SpriteSheet.h>

class Renderer {
public:
    Renderer(const char* title, int width, int height);
    ~Renderer();

    SDL_Renderer* GetSDLRenderer() const;
    void Clear();
    void Present();
    SDL_Texture* LoadTexture(const char* path);
    void RenderTexture(SDL_Texture* texture, int x, int y);
    void RenderSprite(SpriteSheet* spriteSheet, int x, int y, int clipIndex);
    void DestroyTexture(SDL_Texture* texture);

private:
    void CreateWindow(const char* title, int width, int height);
    void CreateRenderer();

    SDL_Window* window;
    SDL_Renderer* m_renderer;
};

#endif // RENDERER_H
