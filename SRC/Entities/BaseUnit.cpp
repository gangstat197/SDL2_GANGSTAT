#include <Entities/BaseUnit.h>
#include <Core/Renderer.h>
#include <SDL_image.h>

BaseUnit::BaseUnit(double x, double y, int width, int height, const std::string& texturePath, int rows, int cols, int frameRate)
    : position(x, y), width(width), height(height), texture(nullptr), texturePath(texturePath) {
    SpriteSheet* spriteSheet = new SpriteSheet(renderer->GetSDLRenderer(), texturePath, rows, cols);
    animation.SetSpriteSheet(spriteSheet);
    animation.SetFrameRate(frameRate);
}

BaseUnit::~BaseUnit() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void BaseUnit::Render(Renderer* renderer) {
    if (!texture) {
        texture = renderer->LoadTexture(texturePath.c_str());
    }
    animation.Render(renderer->GetSDLRenderer(), position.x, position.y);
}

void BaseUnit::Update() {
    animation.Update();
}

void BaseUnit::Clean() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
