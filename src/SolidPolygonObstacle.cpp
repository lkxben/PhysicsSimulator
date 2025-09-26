#include "SolidPolygonObstacle.h"
#include <cmath>
#include <algorithm>

SolidPolygonObstacle::SolidPolygonObstacle(const std::vector<Vector2>& verts, double elasticity_, Color color_)
    : Obstacle(
        std::accumulate(verts.begin(), verts.end(), 0.0, [](double sum, const Vector2& v){ return sum + v.x; }) / verts.size(),
        std::accumulate(verts.begin(), verts.end(), 0.0, [](double sum, const Vector2& v){ return sum + v.y; }) / verts.size(),
        elasticity_,
        color_
      ),
      vertices(verts)
{}

void SolidPolygonObstacle::draw() const {
    if (vertices.size() < 3) return;

    for (size_t i = 0; i < vertices.size(); ++i) {
        Vector2 a = vertices[i];
        Vector2 b = vertices[(i + 1) % vertices.size()];
        DrawLineV(a, b, color);
    }

    for (size_t i = 1; i + 1 < vertices.size(); ++i) {
        DrawTriangle(vertices[0], vertices[i], vertices[i + 1], color);
    }
}

void SolidPolygonObstacle::collide(Particle& p, double dt) const {
    double thickness = 0.01;

    auto pointInPolygon = [&](double px, double py) -> bool {
        int winding = 0;
        for (size_t i = 0; i < vertices.size(); ++i) {
            const Vector2& a = vertices[i];
            const Vector2& b = vertices[(i + 1) % vertices.size()];
            if (a.y <= py) {
                if (b.y > py && (b.x - a.x) * (py - a.y) - (b.y - a.y) * (px - a.x) > 0) ++winding;
            } else {
                if (b.y <= py && (b.x - a.x) * (py - a.y) - (b.y - a.y) * (px - a.x) < 0) --winding;
            }
        }
        return winding != 0;
    };

    if (pointInPolygon(p.x, p.y)) {
        float step = 1.0f;
        while (pointInPolygon(p.x, p.y)) {
            double distLeft = 0, distRight = 0, distUp = 0, distDown = 0;

            while (pointInPolygon(p.x - distLeft, p.y)) distLeft += step;
            while (pointInPolygon(p.x + distRight, p.y)) distRight += step;
            while (pointInPolygon(p.x, p.y - distUp)) distUp += step;
            while (pointInPolygon(p.x, p.y + distDown)) distDown += step;

            double minDist = distLeft;
            int dir = 0;
            if (distRight < minDist) { minDist = distRight; dir = 1; }
            if (distUp < minDist) { minDist = distUp; dir = 2; }
            if (distDown < minDist) { minDist = distDown; dir = 3; }

            switch (dir) {
                case 0: p.x -= minDist; break;
                case 1: p.x += minDist; break;
                case 2: p.y -= minDist; break;
                case 3: p.y += minDist; break;
            }
        }
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        Vector2 a = vertices[i];
        Vector2 b = vertices[(i + 1) % vertices.size()];
        Vector2 edge = {b.x - a.x, b.y - a.y};
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
