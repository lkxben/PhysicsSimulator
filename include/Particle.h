#pragma once
#include <cstddef>
#include "Entity.h"

struct Particle : public Entity {
    float radius;

    Particle(double x_, double y_, double vx_, double vy_, double mass_ = 1.0, float radius_ = 5.0, double elasticity_ = 1.0)
        : Entity(x_, y_, vx_, vy_, mass_, elasticity_), radius(radius_) {}

    inline void move(double dt) {
        x += vx * dt;
        y += vy * dt;
    }
};
