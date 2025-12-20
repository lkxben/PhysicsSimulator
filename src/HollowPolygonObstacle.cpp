#include "HollowPolygonObstacle.h"
#include <cmath>

HollowPolygonObstacle::HollowPolygonObstacle(const std::vector<Vector2>& verts, double thickness_, double elasticity_, Color color_)
    : Obstacle(
        std::accumulate(verts.begin(), verts.end(), 0.0f, [](float sum, const Vector2& v){ return sum + v.x; }) / verts.size(),
        std::accumulate(verts.begin(), verts.end(), 0.0f, [](float sum, const Vector2& v){ return sum + v.y; }) / verts.size(),
        elasticity_,
        color_
    ),
    vertices(verts),
    thickness(thickness_)
{}

void HollowPolygonObstacle::draw() const {
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vector2 a = { static_cast<float>(vertices[i].x), static_cast<float>(vertices[i].y) };
        Vector2 b = { static_cast<float>(vertices[(i + 1) % vertices.size()].x),
                      static_cast<float>(vertices[(i + 1) % vertices.size()].y) };
        DrawLineEx(a, b, static_cast<float>(thickness), color);
    }
}

void HollowPolygonObstacle::collide(Particle& p, double dt) const {
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vector2 a = vertices[i];
        Vector2 b = vertices[(i + 1) % vertices.size()];
        Vector2 edge = { b.x - a.x, b.y - a.y };
        double length = std::hypot(edge.x, edge.y);
        double angle = std::atan2(edge.y, edge.x);

        SolidRectObstacle edgeRect(
            (a.x + b.x) / 2.0,
            (a.y + b.y) / 2.0,
            length,
            thickness,
            angle,
            elasticity
        );

        double oldX = p.x, oldY = p.y;
        double oldVx = p.vx, oldVy = p.vy;

        edgeRect.collide(p, dt);

        if (p.x != oldX || p.y != oldY || p.vx != oldVx || p.vy != oldVy) {
            return;
        }
    }
}
