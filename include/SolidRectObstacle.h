#pragma once
#include <cstddef>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "Particle.h"

struct SolidRectObstacle : public Obstacle {
    double width;
    double height;
    double rotation;

    SolidRectObstacle(double x_, double y_, double width_, double height_, double rotation_ = 0.0, double elasticity_ = 1.0, sf::Color color_ = sf::Color::White);
    virtual ~SolidRectObstacle() = default;

    void draw(sf::RenderWindow& window) const override;
    void collide(Particle& p, double dt) const override;
};
