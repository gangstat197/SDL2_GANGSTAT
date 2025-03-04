#include <Utils/SpriteSheet.h>
#include <SDL_image.h>

SpriteSheet::SpriteSheet(SDL_Renderer* renderer) : mTexture(nullptr), mRenderer(renderer), mWidth(0), mHeight(0) {}

SpriteSheet::SpriteSheet(SDL_Renderer* renderer, const std::string& path, int rows, int cols)
    : mTexture(nullptr), mRenderer(renderer), mWidth(0), mHeight(0) {
    if (LoadFromFile(path)) {
        SplitSheet(rows, cols);
    }
}

SpriteSheet::~SpriteSheet() {
    Free();
}

bool SpriteSheet::LoadFromFile(const std::string& path) {
    Free();

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        SDL_Log("Unable to load image %s! SDL_image Error: %s", path.c_str(), IMG_GetError());
        return false;
    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

    mTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    if (mTexture == nullptr) {
        SDL_Log("Unable to create texture from %s! SDL Error: %s", path.c_str(), SDL_GetError());
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);
    return true;
}

void SpriteSheet::Free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void SpriteSheet::Render(int x, int y, SDL_Rect* clip) {
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    if (clip != nullptr) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(mRenderer, mTexture, clip, &renderQuad);
}

void SpriteSheet::AddClip(int x, int y, int w, int h) {
    SDL_Rect clip = { x, y, w, h };
    mClips.push_back(clip);
}

const std::vector<SDL_Rect>& SpriteSheet::GetClips() const {
    return mClips;
}

void SpriteSheet::SplitSheet(int rows, int cols) {
    int clipWidth = mWidth / cols;
    int clipHeight = mHeight / rows;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            AddClip(col * clipWidth, row * clipHeight, clipWidth, clipHeight);
        }
    }
}

void SpriteSheet::DestroyTexture() {
	if (mTexture) {
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}
}