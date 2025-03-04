#include <Entities/Monster.h>

Monster::Monster(double x, double y, int width, int height, const std::string& texturePath)
    : BaseUnit(x, y, width, height, texturePath) {}

Monster::~Monster() {}

void Monster::Move() {
    // Implement movement logic
}

void Monster::Attack() {
    // Implement attack logic
}

void Monster::Defend() {
    // Implement defend logic
}
