#ifndef MONSTER_H
#define MONSTER_H

#include <Entities/BaseUnit.h>

class Monster : public BaseUnit {
public:
    Monster(double x, double y, int width, int height, const std::string& texturePath);
    ~Monster();

    void Move() override;
    void Attack() override;
    void Defend() override;
};

#endif // MONSTER_H
