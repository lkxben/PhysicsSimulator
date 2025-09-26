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

    std::vector<int> indices(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) indices[i] = i;

    auto cross = [](Vector2 a, Vector2 b) { return a.x * b.y - a.y * b.x; };
    auto isConvex = [&](int i0, int i1, int i2) {
        Vector2 a = vertices[i0];
        Vector2 b = vertices[i1];
        Vector2 c = vertices[i2];
        Vector2 ab = { b.x - a.x, b.y - a.y };
        Vector2 bc = { c.x - b.x, c.y - b.y };
        return cross(ab, bc) < 0;
    };

    auto pointInTriangle = [&](Vector2 p, Vector2 a, Vector2 b, Vector2 c) {
        Vector2 v0 = { c.x - a.x, c.y - a.y };
        Vector2 v1 = { b.x - a.x, b.y - a.y };
        Vector2 v2 = { p.x - a.x, p.y - a.y };
        float dot00 = v0.x*v0.x + v0.y*v0.y;
        float dot01 = v0.x*v1.x + v0.y*v1.y;
        float dot02 = v0.x*v2.x + v0.y*v2.y;
        float dot11 = v1.x*v1.x + v1.y*v1.y;
        float dot12 = v1.x*v2.x + v1.y*v2.y;
        float invDenom = 1.f / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
        return (u >= 0) && (v >= 0) && (u + v <= 1);
    };

    std::vector<int> remaining = indices;
    while (remaining.size() >= 3) {
        bool earFound = false;
        for (size_t i = 0; i < remaining.size(); ++i) {
            int i0 = remaining[(i + remaining.size() - 1) % remaining.size()];
            int i1 = remaining[i];
            int i2 = remaining[(i + 1) % remaining.size()];

            if (!isConvex(i0, i1, i2)) continue;

            bool anyInside = false;
            for (int j : remaining) {
                if (j == i0 || j == i1 || j == i2) continue;
                if (pointInTriangle(vertices[j], vertices[i0], vertices[i1], vertices[i2])) {
                    anyInside = true;
                    break;
                }
            }

            if (!anyInside) {
                DrawTriangle(vertices[i0], vertices[i1], vertices[i2], color);
                remaining.erase(remaining.begin() + i);
                earFound = true;
                break;
            }
        }
        if (!earFound) break;
    }

    // Draw outline for visibility
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vector2 a = vertices[i];
        Vector2 b = vertices[(i + 1) % vertices.size()];
        DrawLineEx(a, b, 1.0f, color);
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
