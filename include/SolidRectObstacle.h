#pragma once
#include <cstddef>
#include "Obstacle.h"

struct SolidRectObstacle : public Obstacle {
    double width;
    double height;
    double rotation;

    SolidRectObstacle(double x_, double y_, double width_, double height_, double rotation_ = 0.0, double elasticity_ = 1.0)
        : Obstacle(x_, y_, elasticity_), width(width_), height(height_), rotation(rotation_) {}
        
    virtual ~SolidRectObstacle() = default;

    void draw(sf::RenderWindow& window) const override {
        sf::RectangleShape shape{sf::Vector2f(static_cast<float>(width), static_cast<float>(height))};
        shape.setOrigin({static_cast<float>(width)/2, static_cast<float>(height)/2});
        shape.setPosition({static_cast<float>(x), static_cast<float>(y)});
        shape.setRotation(sf::degrees(static_cast<float>(rotation * 180.0 / M_PI)));
        window.draw(shape);
    }

    void collide(Particle& p, double dt) const override {
        double cosA = std::cos(rotation);
        double sinA = std::sin(rotation);

        double relX = p.x - x;
        double relY = p.y - y;

        double localX =  cosA * relX + sinA * relY;
        double localY = -sinA * relX + cosA * relY;

        double halfW = width / 2.0;
        double halfH = height / 2.0;
        double clampedX = std::clamp(localX, -halfW, halfW);
        double clampedY = std::clamp(localY, -halfH, halfH);

        double closestX = cosA * clampedX - sinA * clampedY + x;
        double closestY = sinA * clampedX + cosA * clampedY + y;

        // Vector from closest point to particle
        double dx = p.x - closestX;
        double dy = p.y - closestY;
        double dist2 = dx*dx + dy*dy;

        if (dist2 < p.radius * p.radius) {
            double dist = std::sqrt(dist2);
            if (dist == 0.0) { dx = 1e-8; dy = 0; dist = 1e-8; }

            double nx = dx / dist;
            double ny = dy / dist;

            double overlap = p.radius - dist;
            p.x += nx * overlap;
            p.y += ny * overlap;

            double vDotN = p.vx * nx + p.vy * ny;
            if (vDotN < 0) {
                double e = 0.5 * (p.elasticity + elasticity);
                p.vx -= (1 + e) * vDotN * nx;
                p.vy -= (1 + e) * vDotN * ny;
            }
        }
    }
};
