#include <managers/AssetManager.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

AssetManager& AssetManager::Instance() {
    static AssetManager instance;
    return instance;
}

AssetManager::~AssetManager() {
    UnloadAll();
}

SDL_Texture* AssetManager::LoadTexture(const std::string& assetId, const std::string& filePath, SDL_Renderer* renderer) {
    auto it = m_textureMap.find(assetId);
    if (it != m_textureMap.end()) {
        return it->second;
    }

    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
    if (!texture) {
        std::cerr << "AssetManager::LoadTexture - Failed to load: " << filePath << " - " << SDL_GetError() << std::endl;
        return nullptr;
    }

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

int AssetManager::GetTextureWidth(const std::string& assetId) const {
    auto it = m_textureMap.find(assetId);
    if (it != m_textureMap.end()) {
        int width;
        SDL_QueryTexture(it->second, nullptr, nullptr, &width, nullptr);
        return width;
    }
    return 0;
}

int AssetManager::GetTextureHeight(const std::string& assetId) const {
    auto it = m_textureMap.find(assetId);
    if (it != m_textureMap.end()) {
        int height;
        SDL_QueryTexture(it->second, nullptr, nullptr, nullptr, &height);
        return height;
    }
    return 0;
}

SDL_Texture* AssetManager::ScaleTexture(const std::string& assetId, SDL_Texture& texture, SDL_Renderer* renderer, int width, int height) {
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

    m_textureMap[assetId] = scaledTexture; 
    return scaledTexture;
}

SDL_Texture* AssetManager::ScaleTexture(const std::string& assetId, SDL_Texture& texture, SDL_Renderer* renderer, double ratio) {
    int width, height;
    SDL_QueryTexture(&texture, nullptr, nullptr, &width, &height);
    return ScaleTexture(assetId, texture, renderer, static_cast<int>(width * ratio), static_cast<int>(height * ratio));
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
    
    for (auto& pair : m_fontMap) {
        TTF_CloseFont(pair.second);
    }
    m_fontMap.clear();
    
    for (auto& pair : m_soundMap) {
        Mix_FreeChunk(pair.second);
    }
    m_soundMap.clear();
    
    for (auto& pair : m_musicMap) {
        Mix_FreeMusic(pair.second);
    }
    m_musicMap.clear();
}

void AssetManager::AddTexture(const std::string& assetId, SDL_Texture* texture) {
    auto it = m_textureMap.find(assetId);
    if (it != m_textureMap.end()) {
        SDL_DestroyTexture(it->second);
        m_textureMap.erase(it);
    }
    
    m_textureMap[assetId] = texture;
}

TTF_Font* AssetManager::LoadFont(const std::string& fontId, const std::string& filePath, int fontSize) {
    auto it = m_fontMap.find(fontId);
    if (it != m_fontMap.end()) {
        return it->second;
    }

    TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);
    if (!font) {
        std::cerr << "AssetManager::LoadFont - Failed to load: " << filePath << " - " << TTF_GetError() << std::endl;
        return nullptr;
    }

    m_fontMap[fontId] = font;
    return font;
}

void AssetManager::AddFont(const std::string& fontId, TTF_Font* font) {
    auto it = m_fontMap.find(fontId);
    if (it != m_fontMap.end()) {
        TTF_CloseFont(it->second);
        m_fontMap.erase(it);
    }
    
    m_fontMap[fontId] = font;
}

TTF_Font* AssetManager::GetFont(const std::string& fontId) const {
    auto it = m_fontMap.find(fontId);
    if (it != m_fontMap.end()) {
        return it->second;
    }
    return nullptr;
}

void AssetManager::UnloadFont(const std::string& fontId) {
    auto it = m_fontMap.find(fontId);
    if (it != m_fontMap.end()) {
        TTF_CloseFont(it->second);
        m_fontMap.erase(it);
    }
}

Mix_Chunk* AssetManager::LoadSound(const std::string& soundId, const std::string& filePath) {
    auto it = m_soundMap.find(soundId);
    if (it != m_soundMap.end()) {
        return it->second;
    }

    Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
    if (!sound) {
        std::cerr << "AssetManager::LoadSound - Failed to load: " << filePath << " - " << Mix_GetError() << std::endl;
        return nullptr;
    }

    m_soundMap[soundId] = sound;
    return sound;
}

void AssetManager::AddSound(const std::string& soundId, Mix_Chunk* sound) {
    auto it = m_soundMap.find(soundId);
    if (it != m_soundMap.end()) {
        Mix_FreeChunk(it->second);
        m_soundMap.erase(it);
    }
    
    m_soundMap[soundId] = sound;
}

Mix_Chunk* AssetManager::GetSound(const std::string& soundId) const {
    auto it = m_soundMap.find(soundId);
    if (it != m_soundMap.end()) {
        return it->second;
    }
    return nullptr;
}

void AssetManager::UnloadSound(const std::string& soundId) {
    auto it = m_soundMap.find(soundId);
    if (it != m_soundMap.end()) {
        Mix_FreeChunk(it->second);
        m_soundMap.erase(it);
    }
}

Mix_Music* AssetManager::LoadMusic(const std::string& musicId, const std::string& filePath) {
    auto it = m_musicMap.find(musicId);
    if (it != m_musicMap.end()) {
        return it->second;
    }

    Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    if (!music) {
        std::cerr << "AssetManager::LoadMusic - Failed to load: " << filePath << " - " << Mix_GetError() << std::endl;
        return nullptr;
    }

    m_musicMap[musicId] = music;
    return music;
}

void AssetManager::AddMusic(const std::string& musicId, Mix_Music* music) {
    auto it = m_musicMap.find(musicId);
    if (it != m_musicMap.end()) {
        Mix_FreeMusic(it->second);
        m_musicMap.erase(it);
    }
    
    m_musicMap[musicId] = music;
}

Mix_Music* AssetManager::GetMusic(const std::string& musicId) const {
    auto it = m_musicMap.find(musicId);
    if (it != m_musicMap.end()) {
        return it->second;
    }
    return nullptr;
}

void AssetManager::UnloadMusic(const std::string& musicId) {
    auto it = m_musicMap.find(musicId);
    if (it != m_musicMap.end()) {
        Mix_FreeMusic(it->second);
        m_musicMap.erase(it);
    }
}

SDL_Texture* AssetManager::LoadTexture(const std::string& assetId, const std::string& filePath, SDL_Renderer* renderer, double ratio) {
    auto it = m_textureMap.find(assetId);
    if (it != m_textureMap.end()) {
        return it->second;
    }
    
    std::string tempId = assetId + "_base";
    SDL_Texture* baseTexture = LoadTexture(tempId, filePath, renderer);
    
    if (!baseTexture) {
        return nullptr;
    }
    
    SDL_Texture* scaledTexture = ScaleTexture(assetId, *baseTexture, renderer, ratio);
    
    UnloadTexture(tempId);
    
    return scaledTexture;
}

