#pragma once
#include <cstddef>
#include <vector>
#include <numeric>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "Particle.h"
#include "SolidRectObstacle.h"

struct HollowPolygonObstacle : public Obstacle {
    std::vector<sf::Vector2f> vertices;
    double thickness;

    HollowPolygonObstacle(const std::vector<sf::Vector2f>& verts, double thickness_, double elasticity_ = 1.0, sf::Color color_ = sf::Color::White);
    virtual ~HollowPolygonObstacle() = default;

    void draw(sf::RenderWindow& window) const override;
    void collide(Particle& p, double dt) const override;
};
