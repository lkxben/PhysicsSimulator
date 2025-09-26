#pragma once
#include <cstddef>
#include "Obstacle.h"
#include "Particle.h"

struct SolidRectObstacle : public Obstacle {
    double width;
    double height;
    double rotation;

    SolidRectObstacle(double x_, double y_, double width_, double height_, double rotation_ = 0.0, double elasticity_ = 1.0, Color color_ = WHITE);
    virtual ~SolidRectObstacle() = default;

    void draw() const override;
    void collide(Particle& p, double dt) const override;
};
