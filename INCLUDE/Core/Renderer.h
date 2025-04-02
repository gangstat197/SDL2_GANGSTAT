#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <string>
#include <utils/SpriteSheet.h>

class Renderer {
public:
    Renderer(const char* title, int width, int height);
    ~Renderer();

    SDL_Renderer* GetSDLRenderer() const;
    void Clear();
    void Present();

    SDL_Texture* LoadTexture(const char* path);

    SDL_Texture* ScaleTexture(SDL_Texture& texture, int width, int height);
    // SDL_Texture* LoadTextureFromSurface(SDL_Surface* surface);


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
