#pragma once
#include <cstddef>
#include <vector>
#include <numeric>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "Particle.h"
#include "SolidRectObstacle.h"

struct SolidPolygonObstacle : public Obstacle {
    std::vector<sf::Vector2f> vertices;

    SolidPolygonObstacle(const std::vector<sf::Vector2f>& verts, double elasticity_ = 1.0);
    virtual ~SolidPolygonObstacle() = default;

    void draw(sf::RenderWindow& window) const override;
    void collide(Particle& p, double dt) const override;
};
