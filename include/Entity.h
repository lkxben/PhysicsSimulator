#pragma once
#include <cstddef>

struct Entity {
    double x, y;
    double vx, vy;
    double mass;

    Entity(double x_, double y_, double vx_, double vy_, double mass_)
        : x(x_), y(y_), vx(vx_), vy(vy_), mass(mass_) {}

    virtual ~Entity() = default;
};
