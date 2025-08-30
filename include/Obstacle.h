#pragma once
#include <cstddef>
#include <limits>
#include "Entity.h"

struct Obstacle : public Entity {
    Obstacle(double x_, double y_, double elasticity_ = 1.0)
        : Entity(x_, y_, 0, 0, std::numeric_limits<double>::infinity(), elasticity_) {}

    virtual ~Obstacle() = default;
};
