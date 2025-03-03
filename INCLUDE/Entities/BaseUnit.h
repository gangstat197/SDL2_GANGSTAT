#ifndef BASEUNIT_H
#define BASEUNIT_H

class BaseUnit {
public:
    virtual void Move() = 0;
    virtual void Attack() = 0;
    virtual void Defend() = 0;
    // Add any other common methods or properties for units

    virtual ~BaseUnit() {}
};

#endif // BASEUNIT_H
