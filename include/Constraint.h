#pragma once
#include "IntegratorType.h"
#include "raylib.h"

struct Constraint {
    virtual void apply(double dt, int iterations, IntegratorType integrator) = 0;
    virtual void draw() const {}
    virtual std::vector<Particle*> getParticles() const { return {}; } 
    virtual ~Constraint() = default;
};
