#pragma once
#include <cstddef>
#include <limits>
#include "Entity.h"

struct Obstacle : public Entity {
    Obstacle(double x_, double y_, double vx_, double vy_)
        : Entity(x_, y_, vx_, vy_, std::numeric_limits<double>::infinity()) {}

    virtual bool checkCollision(const Entity& e) const = 0;
    virtual ~Obstacle() = default;
};
