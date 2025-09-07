#pragma once
#include <cstddef>
#include "Entity.h"

struct ParticleParams {
    double x = 0;
    double y = 0;
    double vx = 0;
    double vy = 0;
    double mass = 1.0;
    float radius = 5.0;
    double elasticity = 1.0;
    double charge = 0.0;
    sf::Color color = sf::Color::White;
};

struct Particle : public Entity {
    float radius;
    double fx, fy;
    double charge;

    Particle(double x_, double y_, double vx_, double vy_, double mass_ = 1.0, float radius_ = 5.0, double elasticity_ = 1.0, sf::Color color_ = sf::Color::White) 
        : Entity(x_, y_, vx_, vy_, mass_, elasticity_, color_), radius(radius_) {}

    Particle(const ParticleParams& params)
        : Entity(params.x, params.y, params.vx, params.vy, params.mass, params.elasticity, params.color), radius(params.radius), charge(params.charge) {}

    virtual ~Particle() = default;

    inline void move(double dt) {
        x += vx * dt;
        y += vy * dt;
    }

    void draw(sf::RenderWindow& window) const override {
        sf::CircleShape shape{radius, 30};
        shape.setPosition({static_cast<float>(x - radius),
                        static_cast<float>(y - radius)});
        shape.setFillColor(color);
        window.draw(shape);
    }
};
