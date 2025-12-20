#pragma once
#include <cstddef>
#include "Obstacle.h"
#include "Particle.h"

struct HollowCircleObstacle : public Obstacle {
    double radius;
    double thickness;

    HollowCircleObstacle(double x_, double y_, double radius_, double thickness_, double elasticity_ = 1.0, Color color_ = WHITE);
    virtual ~HollowCircleObstacle() = default;

    void draw() const override;
    void collide(Particle& p, double dt) const override;
};
