#pragma once
#include <cstddef>
#include <vector>
#include <numeric>
#include "Obstacle.h"
#include "Particle.h"
#include "SolidRectObstacle.h"

struct SolidPolygonObstacle : public Obstacle {
    std::vector<Vector2> vertices;

    SolidPolygonObstacle(const std::vector<Vector2>& verts, double elasticity_ = 1.0, Color color_ = WHITE);
    virtual ~SolidPolygonObstacle() = default;

    void draw() const override;
    void collide(Particle& p, double dt) const override;
};
