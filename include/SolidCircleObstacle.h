#pragma once
#include <cstddef>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "Particle.h"

struct SolidCircleObstacle : public Obstacle {
    double radius;

    SolidCircleObstacle(double x_, double y_, double radius_, double elasticity_ = 1.0, sf::Color color_ = sf::Color::White);
    virtual ~SolidCircleObstacle() = default;

    void draw(sf::RenderWindow& window) const override;
    void collide(Particle& p, double dt) const override;
};
