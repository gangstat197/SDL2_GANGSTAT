#include <Utils/Vector2D.h>
#include <cmath>
// Constructors
Vector2D::Vector2D() : m_x(0.0), m_y(0.0) {}

Vector2D::Vector2D(double x, double y) : m_x(x), m_y(y) {}

// Getters
double Vector2D::getX() const {
    return m_x;
}

double Vector2D::getY() const {
    return m_y;
}

// Setters
void Vector2D::setX(double x) {
    m_x = x;
}

void Vector2D::setY(double y) {
    m_y = y;
}

// Vector operations
Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(m_x + other.m_x, m_y + other.m_y);
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(m_x - other.m_x, m_y - other.m_y);
}

Vector2D Vector2D::operator*(double scalar) const {
    return Vector2D(m_x * scalar, m_y * scalar);
}

double Vector2D::operator*(const Vector2D& other) const {
    return m_x * other.m_x + m_y * other.m_y;
}

double Vector2D::magnitude() const {
    return sqrt(m_x * m_x + m_y * m_y);
}

Vector2D Vector2D::normalize() const {
    double mag = magnitude();
    if (mag != 0.0) {
        return Vector2D(m_x / mag, m_y / mag);
    }
    return Vector2D();
}
