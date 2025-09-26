#pragma once
#include "../../include/Particle.h"

struct ClothParticle : public Particle {
    using Particle::Particle;

    void draw(sf::RenderWindow& window) const override {
        return;
    }
};