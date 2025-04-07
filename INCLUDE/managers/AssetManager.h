#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

class AssetManager {
public:
    static AssetManager& Instance();

    SDL_Texture* LoadTexture(const std::string& assetId, const std::string& filePath, SDL_Renderer* renderer);

    void AddTexture(const std::string& assetId, SDL_Texture* texture);
    
    SDL_Texture* GetTexture(const std::string& assetId) const;
    
    SDL_Texture* ScaleTexture(const std::string& assetId, SDL_Texture& texture, SDL_Renderer* renderer, int width, int height);
    SDL_Texture* ScaleTexture(const std::string& assetId, SDL_Texture& texture, SDL_Renderer* renderer, double ratio);

    void UnloadTexture(const std::string& assetId);

    void UnloadAll();

private:
    AssetManager() = default;
    ~AssetManager();

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    std::unordered_map<std::string, SDL_Texture*> m_textureMap;
};