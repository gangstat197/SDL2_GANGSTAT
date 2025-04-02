#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

class AssetManager {
public:
    // Returns the singleton instance
    static AssetManager& Instance();

    // Loads a texture from file if not already loaded
    // Returns the SDL_Texture pointer from the cache
    SDL_Texture* LoadTexture(const std::string& assetId, const std::string& filePath, SDL_Renderer* renderer);

    
    // Retrieves a texture from the cache by ID
    SDL_Texture* GetTexture(const std::string& assetId) const;
    
    // Scales a texture to the specified width and height
    SDL_Texture* ScaleTexture(SDL_Texture& texture, SDL_Renderer* renderer, int width, int height);

    // Destroys a loaded texture, removes it from the cache
    void UnloadTexture(const std::string& assetId);

    // Destroys all loaded textures
    void UnloadAll();

private:
    AssetManager() = default;
    ~AssetManager();

    // Delete copy and move operations
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    std::unordered_map<std::string, SDL_Texture*> m_textureMap;
};