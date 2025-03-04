#ifndef BASEUNIT_H
#define BASEUNIT_H

#include <SDL.h>
#include <string>
#include <Utils/Vector2D.h>
#include <Core/Renderer.h>

class BaseUnit {
public:
    BaseUnit(double x, double y, int width, int height, const std::string& texturePath);
    virtual ~BaseUnit();

    virtual void Move() = 0;
    virtual void Attack() = 0;
    virtual void Defend() = 0;

    void Render(Renderer* renderer);
    void Clean();

protected:
    Vector2D position;
    int width, height;
    SDL_Texture* texture;
    std::string texturePath;
};

#endif // BASEUNIT_H

