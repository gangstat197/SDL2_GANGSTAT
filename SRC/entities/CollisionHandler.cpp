#include <entities/CollisionHandler.h>
#include <entities/Entity.h>
#include <cmath>

CollisionHandler::CollisionHandler() {}

CollisionHandler::~CollisionHandler() {}

bool CollisionHandler::CheckRectCollision(const SDL_Rect& rectA, const SDL_Rect& rectB) {
    return SDL_HasIntersection(&rectA, &rectB);
}

bool CollisionHandler::CheckCircleCollision(const CollisionCircle& circleA, const CollisionCircle& circleB) {
    // Calculate squared distance between centers
    float distanceSquared = DistanceSquared(circleA.center, circleB.center);
    
    // Calculate squared sum of radii
    float radiiSumSquared = (circleA.radius + circleB.radius) * (circleA.radius + circleB.radius);
    
    // If distance squared is less than or equal to the squared sum of radii, circles overlap
    return distanceSquared <= radiiSumSquared;
}

bool CollisionHandler::CheckCircleRectCollision(const CollisionCircle& circle, const SDL_Rect& rect) {
    // Find the closest point on the rectangle to the circle's center
    float closestX = std::max(static_cast<float>(rect.x), std::min(static_cast<float>(circle.center.x), static_cast<float>(rect.x + rect.w)));
    float closestY = std::max(static_cast<float>(rect.y), std::min(static_cast<float>(circle.center.y), static_cast<float>(rect.y + rect.h)));
    
    // Calculate squared distance between the closest point and the circle's center
    Vector2D closestPoint(closestX, closestY);
    float distanceSquared = DistanceSquared(circle.center, closestPoint);
    
    // If distance squared is less than or equal to the squared radius, there's a collision
    return distanceSquared <= (circle.radius * circle.radius);
}

bool CollisionHandler::IsPointInRect(const Vector2D& point, const SDL_Rect& rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.w &&
            point.y >= rect.y && point.y <= rect.y + rect.h);
}

bool CollisionHandler::IsPointInCircle(const Vector2D& point, const CollisionCircle& circle) {
    // Calculate squared distance between point and circle center
    float distanceSquared = DistanceSquared(point, circle.center);
    
    // If distance squared is less than or equal to the squared radius, point is inside the circle
    return distanceSquared <= (circle.radius * circle.radius);
}

bool CollisionHandler::IsPointInPolygon(const Vector2D& point, const CollisionPolygon& polygon) {
    if (polygon.points.size() < 3) {
        return false; // Not a valid polygon
    }
    
    // Ray casting algorithm - count intersections of a ray from the point to the right
    bool inside = false;
    
    for (size_t i = 0, j = polygon.points.size() - 1; i < polygon.points.size(); j = i++) {
        bool intersects = 
            ((polygon.points[i].y > point.y) != (polygon.points[j].y > point.y)) &&
            (point.x < (polygon.points[j].x - polygon.points[i].x) * (point.y - polygon.points[i].y) / 
                      (polygon.points[j].y - polygon.points[i].y) + polygon.points[i].x);
        
        if (intersects) {
            inside = !inside;
        }
    }
    
    return inside;
}

CollisionCircle CollisionHandler::CreateCircleFromEntity(const Entity& entity) {
    CollisionCircle circle;
    circle.center = entity.GetPosition();
    
    // Use the smaller dimension (width or height) for the radius
    // Multiply by 0.5 to get a reasonable circle from the dimensions
    int width = entity.GetWidth();
    int height = entity.GetHeight();
    float smallerDimension = std::min(width, height);
    
    circle.radius = smallerDimension * 0.5f * entity.GetScale();
    
    return circle;
}

float CollisionHandler::DistanceSquared(const Vector2D& pointA, const Vector2D& pointB) {
    float dx = pointB.x - pointA.x;
    float dy = pointB.y - pointA.y;
    
    return dx * dx + dy * dy;
}
