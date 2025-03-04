#include <Entities/BaseUnit.h>
#include <Core/Renderer.h>
#include <SDL_image.h>

BaseUnit::BaseUnit(double x, double y, int width, int height, const std::string& texturePath)
    : position(x, y), width(width), height(height), texture(nullptr), texturePath(texturePath) {}

BaseUnit::~BaseUnit() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void BaseUnit::Render(Renderer* renderer) {
    if (!texture) {
        texture = renderer->LoadTexture(texturePath.c_str());
    }
    renderer->RenderTexture(texture, position.x, position.y);
}

void BaseUnit::Clean() {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

