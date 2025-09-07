#pragma once
#include <cstddef>
#include <limits>
#include "Entity.h"
#include "Particle.h"

struct Obstacle : public Entity {
    Obstacle(double x_, double y_, double elasticity_ = 1.0, sf::Color color_ = sf::Color::White)
        : Entity(x_, y_, 0, 0, std::numeric_limits<double>::infinity(), elasticity_, color_) {}

    virtual ~Obstacle() = default;
    virtual void collide(Particle& p, double dt) const = 0;
};
