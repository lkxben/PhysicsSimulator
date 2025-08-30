#pragma once
#include <cstddef>
#include "Obstacle.h"

struct LineObstacle: public Obstacle {
    double length;
    double angle;

    LineObstacle(double x_, double y_, double length_, double angle_ = 0.0)
        : Obstacle(x_, y_), length(length_), angle(angle_) {}
        
    virtual ~LineObstacle() = default;
};
