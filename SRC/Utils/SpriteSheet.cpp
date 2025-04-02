#include <utils/SpriteSheet.h>
#include <SDL2/SDL_image.h>

SpriteSheet::SpriteSheet(SDL_Renderer* renderer)
    : mTexture(nullptr), mRenderer(renderer), mWidth(0), mHeight(0) {
}

SpriteSheet::SpriteSheet(SDL_Renderer* renderer, const std::string& path, int rows, int cols)
    : mTexture(nullptr), mRenderer(renderer), mWidth(0), mHeight(0) {
    LoadFromFile(path);
    SplitSheet(rows, cols);
}

SpriteSheet::~SpriteSheet() {
    DestroyTexture();
}

bool SpriteSheet::LoadFromFile(const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        return false;
    }
    mTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
    mWidth = surface->w;
    mHeight = surface->h;
    SDL_FreeSurface(surface);
    return (mTexture != nullptr);
}

void SpriteSheet::Free() {
    DestroyTexture();
    mWidth = 0;
    mHeight = 0;
}

void SpriteSheet::Render(int x, int y, SDL_Rect* clip) {
    SDL_Rect dst = { x, y, 0, 0 };
    if (clip) {
        dst.w = clip->w;
        dst.h = clip->h;
    } else {
        dst.w = mWidth;
        dst.h = mHeight;
    }
    SDL_RenderCopy(mRenderer, mTexture, clip, &dst);
}

void SpriteSheet::AddClip(int x, int y, int w, int h) {
    SDL_Rect rect{ x, y, w, h };
    mClips.push_back(rect);
}

void SpriteSheet::DestroyTexture() {
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}

const std::vector<SDL_Rect>& SpriteSheet::GetClips() const {
    return mClips;
}

void SpriteSheet::SplitSheet(int rows, int cols) {
    int clipW = mWidth / cols;
    int clipH = mHeight / rows;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            AddClip(c * clipW, r * clipH, clipW, clipH);
        }
    }
}
