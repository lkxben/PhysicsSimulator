#pragma once
#include "../../include/Particle.h"

struct Ball : public Particle {
    bool alive = true;

    using Particle::Particle;

    void draw(sf::RenderWindow& window) const override {
        if (!alive) return;

        sf::CircleShape shape{radius, 30};
        shape.setOrigin({radius, radius});
        shape.setPosition({static_cast<float>(x), static_cast<float>(y)});
        shape.setFillColor(color);
        window.draw(shape);
    }
};