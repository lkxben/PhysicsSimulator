#pragma once
#include <cstddef>
#include <vector>
#include <numeric>
#include "Obstacle.h"
#include "Particle.h"
#include "SolidRectObstacle.h"

struct HollowPolygonObstacle : public Obstacle {
    std::vector<Vector2> vertices;
    double thickness;

    HollowPolygonObstacle(const std::vector<Vector2>& verts, double thickness_, double elasticity_ = 1.0, Color color_ = WHITE);
    virtual ~HollowPolygonObstacle() = default;

    void draw() const override;
    void collide(Particle& p, double dt) const override;
};
