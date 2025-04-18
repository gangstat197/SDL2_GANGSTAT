#include <utils/Vector2D.h>
#include <iostream>
#include <cmath>

// Constructors
Vector2D::Vector2D() : x(0.0), y(0.0) {}

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

// Vector operations
Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator*(double scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

double Vector2D::operator*(const Vector2D& other) const {
    return x * other.x + y * other.y;
}

double Vector2D::magnitude() const {
    return sqrt(x * x + y * y);
}

Vector2D Vector2D::normalize() const {
    double mag = magnitude();
    if (mag != 0.0) {
        return Vector2D(x / mag, y / mag);
    }
    return Vector2D();
}

Vector2D Vector2D::perpendicular() const {
    return Vector2D(-y, x);
}

void Vector2D::print() const {
    std::cerr << x << ',' << y << '\n';
}
