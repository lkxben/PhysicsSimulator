#pragma once
#include <cstddef>
#include "Obstacle.h"

struct SolidCircleObstacle : public Obstacle {
    double radius;

    SolidCircleObstacle(double x_, double y_, double radius_, double elasticity_ = 1.0)
        : Obstacle(x_, y_, elasticity_), radius(radius_) {}
        
    virtual ~SolidCircleObstacle() = default;
};
