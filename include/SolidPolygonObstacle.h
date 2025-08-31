#pragma once
#include <cstddef>
#include "Obstacle.h"
#include "Particle.h"
#include <numeric>

struct SolidPolygonObstacle : public Obstacle {
    std::vector<sf::Vector2f> vertices;

    SolidPolygonObstacle(const std::vector<sf::Vector2f>& verts, double elasticity_ = 1.0)
        : Obstacle(
            std::accumulate(verts.begin(), verts.end(), 0.0, [](double sum, const sf::Vector2f& v){ return sum + v.x; }) / verts.size(),
            std::accumulate(verts.begin(), verts.end(), 0.0, [](double sum, const sf::Vector2f& v){ return sum + v.y; }) / verts.size(),
            elasticity_
        ),
        vertices(verts)
    {}
        
    virtual ~SolidPolygonObstacle() = default;

    void draw(sf::RenderWindow& window) const override {
        if (vertices.size() < 3) return;

        std::vector<int> indices(vertices.size());
        for (size_t i = 0; i < vertices.size(); ++i) indices[i] = i;

        sf::VertexArray va{sf::PrimitiveType::Triangles};

        auto cross = [](sf::Vector2f a, sf::Vector2f b) { return a.x * b.y - a.y * b.x; };
        auto isConvex = [&](int i0, int i1, int i2) {
            sf::Vector2f a = vertices[i0];
            sf::Vector2f b = vertices[i1];
            sf::Vector2f c = vertices[i2];
            return cross(b - a, c - b) < 0;
        };
        auto pointInTriangle = [&](sf::Vector2f p, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c) {
            sf::Vector2f v0 = c - a, v1 = b - a, v2 = p - a;
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
                    va.append({vertices[i0], sf::Color(150,150,150)});
                    va.append({vertices[i1], sf::Color(150,150,150)});
                    va.append({vertices[i2], sf::Color(150,150,150)});
                    remaining.erase(remaining.begin() + i);
                    earFound = true;
                    break;
                }
            }
            if (!earFound) break;
        }

        window.draw(va);
    }

    void collide(Particle& p, double dt) const override {
        double thickness = 0.01;

        auto pointInPolygon = [&](double px, double py) -> bool {
            int winding = 0;
            for (size_t i = 0; i < vertices.size(); ++i) {
                const sf::Vector2f& a = vertices[i];
                const sf::Vector2f& b = vertices[(i + 1) % vertices.size()];
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
            sf::Vector2f a = vertices[i];
            sf::Vector2f b = vertices[(i + 1) % vertices.size()];
            sf::Vector2f edge = b - a;
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
};