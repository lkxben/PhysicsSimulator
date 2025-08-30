#pragma once
#include <cstddef>
#include "Obstacle.h"

struct CircleObstacle : public Obstacle {
    double radius;

    CircleObstacle(double x_, double y_, double radius_, double elasticity_ = 1.0)
        : Obstacle(x_, y_, elasticity_), radius(radius_) {}
        
    virtual ~CircleObstacle() = default;
};
