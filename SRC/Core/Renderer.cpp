#include <Core/Renderer.h>
#include <SDL_image.h>
#include <Utils/Messages.h>

Renderer::Renderer(SDL_Window* window, int width, int height) {
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
    }
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(m_renderer);
}

void Renderer::Clear() {
    SDL_RenderClear(m_renderer);
}

void Renderer::Present() {
    SDL_RenderPresent(m_renderer);
}

void Renderer::DestroyTexture(SDL_Texture* texture) {
	SDL_DestroyTexture(texture);
}

SDL_Texture* Renderer::LoadTexture(const char* path) {
    SDL_Texture* texture = IMG_LoadTexture(m_renderer, path);
    if (texture == nullptr) {
        LogErrorAndExit("Renderer::LoadTexture", "Failed to load texture");
    }
    return texture;
}

void Renderer::RenderTexture(SDL_Texture* texture, int x, int y) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(m_renderer, texture, nullptr, &dst);
}

SDL_Renderer* Renderer::GetSDLRenderer() const {
    return m_renderer;
}
