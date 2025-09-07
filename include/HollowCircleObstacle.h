#pragma once
#include <cstddef>
#include "Obstacle.h"
#include "Particle.h"
#include <SFML/Graphics.hpp>

struct HollowCircleObstacle : public Obstacle {
    double radius;
    double thickness;

    HollowCircleObstacle(double x_, double y_, double radius_, double thickness_, double elasticity_ = 1.0, sf::Color color_ = sf::Color::White);
    virtual ~HollowCircleObstacle() = default;

    void draw(sf::RenderWindow& window) const override;
    void collide(Particle& p, double dt) const override;
};
