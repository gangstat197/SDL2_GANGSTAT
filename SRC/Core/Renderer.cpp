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
        //LogErrorAndExit("Renderer::LoadTexture", "Failed to load texture");
    }
    return texture;
}

SDL_Texture* Renderer::ScaleTexture(SDL_Texture& texture, int width, int height) {
    // Create a new texture with the target dimensions
    SDL_Texture* scaledTexture = SDL_CreateTexture(m_renderer, 
                                                  SDL_PIXELFORMAT_RGBA8888, 
                                                  SDL_TEXTUREACCESS_TARGET, 
                                                  width, height);
    
    if (scaledTexture == nullptr) {
        SDL_Log("Failed to create scaled texture: %s", SDL_GetError());
        return nullptr;
    }
    
    // Set blend mode to preserve alpha channel
    SDL_SetTextureBlendMode(scaledTexture, SDL_BLENDMODE_BLEND);
    
    // Save current render target
    SDL_Texture* oldTarget = SDL_GetRenderTarget(m_renderer);
    
    // Set the new texture as render 
    if (SDL_SetRenderTarget(m_renderer, scaledTexture) != 0) {
        SDL_Log("Failed to set render target: %s", SDL_GetError());
        SDL_DestroyTexture(scaledTexture);
        return nullptr;
    }
    
    // Clear the new texture
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_renderer);
    
    // Render the original texture onto the new texture
    if (SDL_RenderCopy(m_renderer, &texture, nullptr, nullptr) != 0) {
        SDL_Log("Failed to copy texture: %s", SDL_GetError());
        SDL_SetRenderTarget(m_renderer, oldTarget);
        SDL_DestroyTexture(scaledTexture);
        return nullptr;
    }
    
    // Reset render target to previous
    SDL_SetRenderTarget(m_renderer, oldTarget);
    
    return scaledTexture;
}

void Renderer::RenderTexture(SDL_Texture* texture, int x, int y) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(m_renderer, texture, nullptr, &dst);
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
