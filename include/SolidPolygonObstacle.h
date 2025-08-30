#pragma once
#include <cstddef>
#include "Obstacle.h"

struct SolidPolygonObstacle : public Obstacle {
    double radius;

    SolidPolygonObstacle(double x_, double y_, double radius_, double elasticity_ = 1.0)
        : Obstacle(x_, y_, elasticity_), radius(radius_) {}
        
    virtual ~SolidPolygonObstacle() = default;
};
