#pragma once
#include "../../include/Particle.h"

struct ClothParticle : public Particle {
    using Particle::Particle;

    void draw() const override {
        return;
    }
};