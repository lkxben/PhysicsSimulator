#pragma once
#include <cstddef>
#include "Obstacle.h"

struct SolidRectObstacle : public Obstacle {
    double width;
    double height;
    double rotation;

    SolidRectObstacle(double x_, double y_, double width_, double height_, double rotation_ = 0.0, double elasticity_ = 1.0)
        : Obstacle(x_, y_, elasticity_), width(width_), height(height_), rotation(rotation_) {}
        
    virtual ~SolidRectObstacle() = default;
};
