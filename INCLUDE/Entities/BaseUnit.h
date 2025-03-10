#ifndef BASEUNIT_H
#define BASEUNIT_H

#include <SDL.h>
#include <string>
#include <Utils/Vector2D.h>
#include <Core/Renderer.h>
#include <Utils/Animation.h>

class BaseUnit {
public:
    BaseUnit(double x, double y, int width, int height, const std::string& texturePath, int rows, int cols, int frameRate);
    virtual ~BaseUnit();

    virtual void Move() = 0;
    virtual void Attack() = 0;
    virtual void Defend() = 0;

    void Render(Renderer* renderer);
    void Update();
    void Clean();

protected:
    Vector2D position;
    int width, height;
    SDL_Texture* texture;
    std::string texturePath;
    Animation animation;
};

#endif // BASEUNIT_H
