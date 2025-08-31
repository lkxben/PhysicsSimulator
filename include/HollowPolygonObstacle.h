#pragma once
#include <cstddef>
#include "Obstacle.h"
#include "Particle.h"
#include "SolidRectObstacle.h"
#include <numeric>

struct HollowPolygonObstacle : public Obstacle {
    std::vector<sf::Vector2f> vertices;
    double thickness;

    HollowPolygonObstacle(const std::vector<sf::Vector2f>& verts, double thickness_, double elasticity_ = 1.0)
        : Obstacle(
            std::accumulate(verts.begin(), verts.end(), 0.0, [](double sum, const sf::Vector2f& v){ return sum + v.x; }) / verts.size(),
            std::accumulate(verts.begin(), verts.end(), 0.0, [](double sum, const sf::Vector2f& v){ return sum + v.y; }) / verts.size(),
            elasticity_
        ),
        vertices(verts),
        thickness(thickness_)
    {}

    virtual ~HollowPolygonObstacle() = default;

    void draw(sf::RenderWindow& window) const override {
        sf::ConvexShape shape;
        shape.setPointCount(vertices.size());
        for (size_t i = 0; i < vertices.size(); ++i)
            shape.setPoint(i, vertices[i]);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(static_cast<float>(thickness));
        shape.setOutlineColor(sf::Color(150,150,150));
        window.draw(shape);
    }

    void collide(Particle& p, double dt) const override {
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
