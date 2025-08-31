#pragma once
#include "../../include/Particle.h"

struct CueBall : public Particle {
    bool alive = true;

    CueBall(double x_, double y_, double vx_ = 0.0, double vy_ = 0.0, double mass_ = 1.0, float radius_ = 8.0, double elasticity_ = 1.0, sf::Color color_ = sf::Color::White)
        : Particle(x_, y_, vx_, vy_, mass_, radius_, elasticity_, color_) {}

    void draw(sf::RenderWindow& window) const override {
        if (!alive) return;

        sf::CircleShape shape{radius, 30};
        shape.setOrigin({radius, radius});
        shape.setPosition({static_cast<float>(x), static_cast<float>(y)});
        shape.setFillColor(color);
        window.draw(shape);
    }
};