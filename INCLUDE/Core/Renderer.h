#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

class Renderer {
public:
    Renderer(SDL_Window* window, int width, int height);
    ~Renderer();

    void RenderTexture(SDL_Texture* texture, int x, int y);
    void DestroyTexture(SDL_Texture* texture);
    void Clear();
    void Present();

    SDL_Texture* LoadTexture(const char* path);
    SDL_Renderer* GetSDLRenderer() const;

private:
    SDL_Renderer* m_renderer;
};

#endif // RENDERER_H
