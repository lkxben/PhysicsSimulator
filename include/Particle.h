#pragma once
#include <cstddef>

struct Particle {
    double x, y;
    double vx, vy;
    float mass;
    float radius;

    Particle(double x_, double y_, double vx_, double vy_, float radius_ = 5.0, float mass_ = 1.0)
        : x(x_), y(y_), vx(vx_), vy(vy_), radius(radius_), mass(mass_) {}

    inline void move(double dt) {
        x += vx * dt;
        y += vy * dt;
    }
};
