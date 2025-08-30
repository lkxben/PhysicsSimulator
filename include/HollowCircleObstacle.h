#pragma once
#include <cstddef>
#include "Obstacle.h"
#include "Particle.h"

struct HollowCircleObstacle : public Obstacle {
    double radius;
    double thickness;

    HollowCircleObstacle(double x_, double y_, double radius_, double thickness_, double elasticity_ = 1.0)
        : Obstacle(x_, y_, elasticity_), thickness(thickness_), radius(radius_) {}
        
    virtual ~HollowCircleObstacle() = default;

    void draw(sf::RenderWindow& window) const override {
        sf::CircleShape outer(static_cast<float>(radius), 60);
        outer.setOrigin({static_cast<float>(radius), static_cast<float>(radius)});
        outer.setPosition({static_cast<float>(x), static_cast<float>(y)});
        outer.setFillColor(sf::Color::White);
        window.draw(outer);

        sf::CircleShape inner(static_cast<float>(radius - thickness), 60);
        inner.setOrigin({static_cast<float>(radius - thickness), static_cast<float>(radius - thickness)});
        inner.setPosition({static_cast<float>(x), static_cast<float>(y)});
        inner.setFillColor(sf::Color::Transparent);
        inner.setFillColor(sf::Color::Black);
        window.draw(inner);
    }

    void collide(Particle& p, double dt) const override {
        double prev_x = p.x - p.vx * dt;
        double prev_y = p.y - p.vy * dt;

        double dx = p.x - x;
        double dy = p.y - y;
        double dist = std::sqrt(dx * dx + dy * dy);
        if (dist == 0) {
            dx = 1e-6;
            dy = 0;
            dist = 1e-6;
        }
        double inner = radius - thickness;
        double outer = radius;

        double nx = dx / dist;
        double ny = dy / dist;

        double prev_dx = prev_x - x;
        double prev_dy = prev_y - y;
        double prev_dist = std::sqrt(prev_dx * prev_dx + prev_dy * prev_dy);

        if (dist + p.radius > inner && dist - p.radius < outer) {
            double target_dist = (prev_dist < inner) ? (inner - p.radius) : (outer + p.radius);
            p.x = x + nx * target_dist;
            p.y = y + ny * target_dist;

            double e = 0.5 * (p.elasticity + elasticity);

            double v_n = p.vx * nx + p.vy * ny;
            double v_t_x = p.vx - v_n * nx;
            double v_t_y = p.vy - v_n * ny;

            v_n = -e * v_n;

            double e_t = e;
            v_t_x *= e_t;
            v_t_y *= e_t;

            p.vx = v_n * nx + v_t_x;
            p.vy = v_n * ny + v_t_y;
        }
    }
};