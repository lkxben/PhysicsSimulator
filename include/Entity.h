#pragma once
#include <cstddef>
#include <SFML/Graphics.hpp>

struct Entity {
    double x, y;
    sf::Color color;

    Entity(double x_, double y_, sf::Color color_ = sf::Color::White)
        : x(x_), y(y_), color(color_) {}

    virtual ~Entity() = default;
    virtual void draw(sf::RenderWindow& window) const = 0;
};
