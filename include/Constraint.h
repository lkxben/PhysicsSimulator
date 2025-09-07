#pragma once
#include <SFML/Graphics.hpp>

struct Constraint {
    virtual void apply(double dt) = 0;
    virtual void draw(sf::RenderWindow& window) const {}
    virtual ~Constraint() = default;
};
