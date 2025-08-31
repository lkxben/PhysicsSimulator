#pragma once
#include <cstddef>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "Particle.h"

struct HollowRectObstacle : public Obstacle {
    double width;
    double height;
    double thickness;
    double rotation;

    HollowRectObstacle(double x_, double y_, double width_, double height_, double thickness_ = 1.0, double rotation_ = 0.0, double elasticity_ = 1.0);
    virtual ~HollowRectObstacle() = default;

    void draw(sf::RenderWindow& window) const override;
    void collide(Particle& p, double dt) const override;
};
