#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

class SpriteSheet {
public:
    SpriteSheet(SDL_Renderer* renderer);
    SpriteSheet(SDL_Renderer* renderer, const std::string& path, int rows, int cols);
    ~SpriteSheet();

    bool LoadFromFile(const std::string& path);
    void Free();
    void Render(int x, int y, SDL_Rect* clip = nullptr);
    void AddClip(int x, int y, int w, int h);
    void DestroyTexture();

    const std::vector<SDL_Rect>& GetClips() const; // Add getter for mClips

private:
    void SplitSheet(int rows, int cols);

    SDL_Texture* mTexture;
    SDL_Renderer* mRenderer;
    std::vector<SDL_Rect> mClips; // Define mClips
    int mWidth;
    int mHeight;
};

#endif // SPRITESHEET_H
