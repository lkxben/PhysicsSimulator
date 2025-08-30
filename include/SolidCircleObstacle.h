#pragma once
#include <cstddef>
#include "Obstacle.h"

struct SolidCircleObstacle : public Obstacle {
    double radius;

    SolidCircleObstacle(double x_, double y_, double radius_, double elasticity_ = 1.0)
        : Obstacle(x_, y_, elasticity_), radius(radius_) {}
        
    virtual ~SolidCircleObstacle() = default;

    void draw(sf::RenderWindow& window) const override {
        sf::CircleShape shape{static_cast<float>(radius), 30};
        shape.setPosition({static_cast<float>(x - radius),
                        static_cast<float>(y - radius)});
        window.draw(shape);
    }

    void collide(Particle& p, double dt) const override {
        double dx = p.x - x;
        double dy = p.y - y;
        double dist2 = dx*dx + dy*dy;
        double radiiSum = p.radius + radius;

        if (dist2 < radiiSum * radiiSum) {
            double dist = std::sqrt(dist2);
            if (dist == 0.0) { 
                dx = 1e-8; dy = 0; 
                dist = 1e-8; 
            }

            double nx = dx / dist;
            double ny = dy / dist;

            double relVel = p.vx * nx + p.vy * ny;

            if (relVel < 0) {
                double e = 0.5 * (p.elasticity + elasticity);

                p.vx -= (1 + e) * relVel * nx;
                p.vy -= (1 + e) * relVel * ny;
            }

            double overlap = radiiSum - dist;
            p.x += nx * overlap;
            p.y += ny * overlap;
        }
    }
};
