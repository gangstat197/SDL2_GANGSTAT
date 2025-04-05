#pragma once

#include <SDL2/SDL.h>
#include <utils/Vector2D.h>
#include <vector>

// Forward declarations
class Entity;

// For complex collision shapes
struct CollisionCircle {
    Vector2D center;
    float radius;
};

struct CollisionPolygon {
    std::vector<Vector2D> points;
};

class CollisionHandler {
public:
    CollisionHandler();
    ~CollisionHandler();
    
    // Basic rectangle collision (using SDL_Rect)
    static bool CheckRectCollision(const SDL_Rect& rectA, const SDL_Rect& rectB);
    
    // Circle-based collision
    static bool CheckCircleCollision(const CollisionCircle& circleA, const CollisionCircle& circleB);
    static bool CheckCircleRectCollision(const CollisionCircle& circle, const SDL_Rect& rect);
    
    // Point collision
    static bool IsPointInRect(const Vector2D& point, const SDL_Rect& rect);
    static bool IsPointInCircle(const Vector2D& point, const CollisionCircle& circle);
    static bool IsPointInPolygon(const Vector2D& point, const CollisionPolygon& polygon);
    
    // Utility functions
    static CollisionCircle CreateCircleFromEntity(const Entity& entity);
    static float DistanceSquared(const Vector2D& pointA, const Vector2D& pointB);
};
