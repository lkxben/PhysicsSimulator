#pragma once
#include <cstddef>
#include <limits>
#include "Entity.h"
#include "Particle.h"

struct Obstacle : public Entity {
    double elasticity;

    Obstacle(double x_, double y_, double elasticity_ = 1.0, sf::Color color_ = sf::Color::White)
        : Entity(x_, y_, color_), elasticity(elasticity_) {}

    virtual ~Obstacle() = default;
    virtual void collide(Particle& p, double dt) const = 0;
};
