#pragma once
#include <cstddef>
#include "Obstacle.h"
#include "Particle.h"

struct SolidCircleObstacle : public Obstacle {
    double radius;

    SolidCircleObstacle(double x_, double y_, double radius_, double elasticity_ = 1.0, Color color_ = WHITE);
    virtual ~SolidCircleObstacle() = default;

    void draw() const override;
    void collide(Particle& p, double dt) const override;
};
