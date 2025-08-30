#pragma once
#include <cstddef>
#include <SFML/Graphics.hpp>

struct Entity {
    double x, y;
    double vx, vy;
    double mass;
    double elasticity;

    Entity(double x_, double y_, double vx_, double vy_, double mass_, double elasticity_ = 1.0)
        : x(x_), y(y_), vx(vx_), vy(vy_), mass(mass_), elasticity(elasticity_) {}

    virtual ~Entity() = default;
    virtual void draw(sf::RenderWindow& window) const = 0;
};
