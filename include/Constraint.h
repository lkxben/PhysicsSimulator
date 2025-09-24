#pragma once
#include <SFML/Graphics.hpp>
#include "IntegratorType.h"

struct Constraint {
    virtual void apply(double dt, int iterations, IntegratorType integrator) = 0;
    virtual void draw(sf::RenderWindow& window) const {}
    virtual ~Constraint() = default;
};
