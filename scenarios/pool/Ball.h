#pragma once
#include "../../include/Particle.h"

struct Ball : public Particle {
    bool alive = true;

    using Particle::Particle;

    void draw() const override {
        if (!alive) return;

        Particle::draw();
    }
};