#pragma once
#include <cstddef>
#include "Obstacle.h"

struct LineObstacle : public Obstacle {
    double length;
    double angle;
    const float width = 1.0f;

    LineObstacle(double x_, double y_, double length_, double angle_ = 0.0)
        : Obstacle(x_, y_), length(length_), angle(angle_) {}
        
    virtual ~LineObstacle() = default;
};
