#include <managers/AssetManager.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <iostream>

AssetManager& AssetManager::Instance() {
    static AssetManager instance;
    return instance;
}

AssetManager::~AssetManager() {
    UnloadAll();
}

SDL_Texture* AssetManager::LoadTexture(const std::string& assetId, const std::string& filePath, SDL_Renderer* renderer) {
    // Check if texture already exists
    auto it = m_textureMap.find(assetId);
    if (it != m_textureMap.end()) {
        return it->second;
    }

    // Load new texture
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
    if (!texture) {
        std::cerr << "AssetManager::LoadTexture - Failed to load: " << filePath << " - " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // Cache texture
    m_textureMap[assetId] = texture;
    return texture;
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetId) const {
    auto it = m_textureMap.find(assetId);
    if (it != m_textureMap.end()) {
        return it->second;
    }
    return nullptr;
}

SDL_Texture* AssetManager::ScaleTexture(SDL_Texture& texture, SDL_Renderer* renderer, int width, int height) {
    SDL_Texture* scaledTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!scaledTexture) {
        SDL_Log("Failed to create scaled texture: %s", SDL_GetError());
        return nullptr;
    }

    SDL_SetTextureBlendMode(scaledTexture, SDL_BLENDMODE_BLEND);
    SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);

    if (SDL_SetRenderTarget(renderer, scaledTexture) != 0) {
        SDL_Log("Failed to set render target: %s", SDL_GetError());
        SDL_DestroyTexture(scaledTexture);
        return nullptr;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    if (SDL_RenderCopy(renderer, &texture, nullptr, nullptr) != 0) {
        SDL_Log("Failed to copy texture: %s", SDL_GetError());
        SDL_SetRenderTarget(renderer, oldTarget);
        SDL_DestroyTexture(scaledTexture);
        return nullptr;
    }

    SDL_SetRenderTarget(renderer, oldTarget);
    return scaledTexture;
}

void AssetManager::UnloadTexture(const std::string& assetId) {
    auto it = m_textureMap.find(assetId);
    if (it != m_textureMap.end()) {
        SDL_DestroyTexture(it->second);
        m_textureMap.erase(it);
    }
}

void AssetManager::UnloadAll() {
    for (auto& pair : m_textureMap) {
        SDL_DestroyTexture(pair.second);
    }
    m_textureMap.clear();
}