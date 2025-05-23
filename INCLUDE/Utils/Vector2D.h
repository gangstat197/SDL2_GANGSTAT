#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2D {
public:
    // Constructors
    Vector2D();
    Vector2D(double x, double y);

    // Vector operations
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(double scalar) const;
    
    double operator*(const Vector2D& other) const;
    double magnitude() const;
    Vector2D normalize() const;
    Vector2D perpendicular() const;
    void print() const;

    double x;
    double y;
};

#endif