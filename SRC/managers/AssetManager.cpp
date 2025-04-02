//// filepath: d:\3. Project\3. Games\2. ADVANCED PROGRAMMING\SDL2_GANGSTAT\src\managers\AssetManager.cpp
#include "managers/AssetManager.h"
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
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(filePath.c_str()));
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