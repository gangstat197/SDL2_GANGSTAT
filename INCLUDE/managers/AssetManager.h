#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>

class AssetManager {
public:
    static AssetManager& Instance();

    SDL_Texture* LoadTexture(const std::string& assetId, const std::string& filePath, SDL_Renderer* renderer);
    SDL_Texture* LoadTexture(const std::string& assetId, const std::string& filePath, SDL_Renderer* renderer, double ratio);

    void AddTexture(const std::string& assetId, SDL_Texture* texture);
    
    SDL_Texture* GetTexture(const std::string& assetId) const;
    int GetTextureWidth(const std::string& assetId) const;
    int GetTextureHeight(const std::string& assetId) const;
    
    SDL_Texture* ScaleTexture(const std::string& assetId, SDL_Texture& texture, SDL_Renderer* renderer, int width, int height);
    SDL_Texture* ScaleTexture(const std::string& assetId, SDL_Texture& texture, SDL_Renderer* renderer, double ratio);

    void UnloadTexture(const std::string& assetId);

    TTF_Font* LoadFont(const std::string& fontId, const std::string& filePath, int fontSize);
    void AddFont(const std::string& fontId, TTF_Font* font);
    TTF_Font* GetFont(const std::string& fontId) const;
    void UnloadFont(const std::string& fontId);

    Mix_Chunk* LoadSound(const std::string& soundId, const std::string& filePath);
    void AddSound(const std::string& soundId, Mix_Chunk* sound);
    Mix_Chunk* GetSound(const std::string& soundId) const;
    void UnloadSound(const std::string& soundId);

    Mix_Music* LoadMusic(const std::string& musicId, const std::string& filePath);
    void AddMusic(const std::string& musicId, Mix_Music* music);
    Mix_Music* GetMusic(const std::string& musicId) const;
    void UnloadMusic(const std::string& musicId);

    void UnloadAll();

private:
    AssetManager() = default;
    ~AssetManager();

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    std::unordered_map<std::string, SDL_Texture*> m_textureMap;
    std::unordered_map<std::string, TTF_Font*> m_fontMap;
    std::unordered_map<std::string, Mix_Chunk*> m_soundMap;
    std::unordered_map<std::string, Mix_Music*> m_musicMap;
};