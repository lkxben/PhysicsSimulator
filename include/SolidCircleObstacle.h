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

        double speed2 = p.vx * p.vx + p.vy * p.vy;
        double threshold = p.radius / dt;
        double threshold2 = threshold * threshold;

        if (speed2 > threshold2) {
            double prev_x = p.x - p.vx * dt;
            double prev_y = p.y - p.vy * dt;

            double prev_dx = prev_x - x;
            double prev_dy = prev_y - y;
            double prev_dist = std::sqrt(prev_dx * prev_dx + prev_dy * prev_dy);

            if (prev_dist > radiiSum && dist2 <= radiiSum * radiiSum) {
                double t = (prev_dist - radiiSum) / (prev_dist - std::sqrt(dist2));
                t = std::clamp(t, 0.0, 1.0);

                p.x = prev_x + p.vx * t * dt;
                p.y = prev_y + p.vy * t * dt;

                dx = p.x - x;
                dy = p.y - y;
                dist2 = dx*dx + dy*dy;
            }
        }

        if (dist2 < radiiSum * radiiSum) {
            double dist = std::sqrt(dist2);
            if (dist == 0.0) { dx = 1e-8; dy = 0; dist = 1e-8; }

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
