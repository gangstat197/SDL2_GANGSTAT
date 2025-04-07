#include <core/Renderer.h>
#include <utils/SpriteSheet.h>
#include <SDL2/SDL_image.h>
#include <utils/Messages.h>

Renderer::Renderer(const char* title, int width, int height) {
    CreateWindow(title, width, height);
    CreateRenderer();
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(m_renderer);
}

void Renderer::CreateRenderer() {
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
    }
}

void Renderer::CreateWindow(const char* title, int width, int height) {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
    }

    SDL_Surface* iconSurface = IMG_Load("assets/images/logo_MicroDash.png");
    if (iconSurface == nullptr) {
        SDL_Log("Failed to load icon surface: %s", SDL_GetError());
    } else {
        SDL_SetWindowIcon(window, iconSurface);
        SDL_FreeSurface(iconSurface);
    }
}

void Renderer::Clear() {
    SDL_RenderClear(m_renderer);
}

void Renderer::Present() {
    SDL_RenderPresent(m_renderer);
}

void Renderer::RenderTexture(SDL_Texture* texture, int x, int y) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(m_renderer, texture, nullptr, &dst);
}

void Renderer::RenderTexture(SDL_Texture* texture, int x, int y, int width, int height, float rotation) {
    if (texture) {
        SDL_Rect destRect = {
            static_cast<int>(x - width / 2),
            static_cast<int>(y - height / 2),
            width,
            height
        };
        
        SDL_Point center = {width / 2, height / 2};
        
        SDL_RenderCopyEx(
            m_renderer,
            texture,
            nullptr,
            &destRect,
            rotation,
            &center,
            SDL_FLIP_NONE
        );
    }
}

void Renderer::RenderTexture(SDL_Texture* texture, const SDL_Rect& destRect) {
    if (texture) {
        SDL_RenderCopy(m_renderer, texture, nullptr, &destRect);
    }
}

void Renderer::RenderSprite(SpriteSheet* spriteSheet, int x, int y, int clipIndex) {
    const std::vector<SDL_Rect>& clips = spriteSheet->GetClips();
    if (clipIndex < 0 || clipIndex >= clips.size()) {
        SDL_Log("Invalid clip index");
        return;
    }
    spriteSheet->Render(x, y, const_cast<SDL_Rect*>(&clips[clipIndex]));
}

SDL_Renderer* Renderer::GetSDLRenderer() const {
    return m_renderer;
}

SDL_Window* Renderer::GetWindow() const {
    return window;
}

void Renderer::GetWindowSize(int* width, int* height) const {
    if (window) {
        SDL_GetWindowSize(window, width, height);
    } else {
        *width = 0;
        *height = 0;
    }
}
