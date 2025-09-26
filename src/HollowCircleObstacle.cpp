#include "HollowCircleObstacle.h"
#include <algorithm>
#include <cmath>

HollowCircleObstacle::HollowCircleObstacle(double x_, double y_, double radius_, double thickness_, double elasticity_, Color color_)
    : Obstacle(x_, y_, elasticity_, color_), thickness(thickness_), radius(radius_) {}

void HollowCircleObstacle::draw() const {
    Vector2 center{static_cast<float>(x), static_cast<float>(y)};
    
    DrawCircleLines(center.x, center.y, static_cast<float>(radius), color);

    if (thickness > 0) {
        DrawCircleLines(center.x, center.y, static_cast<float>(radius - thickness), {0, 0, 0, 0});
    }
}

void HollowCircleObstacle::collide(Particle& p, double dt) const {
    double dx = p.x - x;
    double dy = p.y - y;
    double dist = std::sqrt(dx * dx + dy * dy);
    if (dist == 0) { dx = 1e-6; dy = 0; dist = 1e-6; }

    double inner = radius - thickness;
    double outer = radius;

    double speed2 = p.vx * p.vx + p.vy * p.vy;
    double threshold = (std::min(thickness, static_cast<double>(p.radius)) / dt);
    double threshold2 = threshold * threshold;

    if (speed2 > threshold2) {
        double prev_x = p.x - p.vx * dt;
        double prev_y = p.y - p.vy * dt;

        double prev_dx = prev_x - x;
        double prev_dy = prev_y - y;
        double prev_dist = std::sqrt(prev_dx * prev_dx + prev_dy * prev_dy);

        bool wasInside = prev_dist > inner && prev_dist < outer;
        bool isInside = dist > inner && dist < outer;
        if (wasInside == isInside) return;

        double target_dist = (prev_dist < inner) ? (inner - p.radius) : (outer + p.radius);
        double t = (target_dist - prev_dist) / (dist - prev_dist);
        if (t < 0 || t > 1) return;

        p.x = prev_x + p.vx * (t * dt);
        p.y = prev_y + p.vy * (t * dt);

        double nx = dx / dist;
        double ny = dy / dist;

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

        double remaining = (1 - t) * dt;
        p.x += p.vx * remaining;
        p.y += p.vy * remaining;
    } else {
        if (dist + p.radius > inner && dist - p.radius < outer) {
            double prev_x = p.x - p.vx * dt;
            double prev_y = p.y - p.vy * dt;

            double prev_dx = prev_x - x;
            double prev_dy = prev_y - y;
            double prev_dist = std::sqrt(prev_dx * prev_dx + prev_dy * prev_dy);

            double target_dist = (prev_dist < inner) ? (inner - p.radius) : (outer + p.radius);
            double nx = dx / dist;
            double ny = dy / dist;

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
}
