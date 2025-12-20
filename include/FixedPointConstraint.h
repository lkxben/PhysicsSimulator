#pragma once
#include "Constraint.h"
#include "Particle.h"

struct FixedPointConstraint : Constraint {
    Particle* p;
    double fx, fy;

    FixedPointConstraint(Particle* particle, double x, double y)
        : p(particle), fx(x), fy(y) {}

    std::vector<Particle*> getParticles() const override {
        return {p};
    }

    void apply(double dt, int iterations, IntegratorType integrator) override {
        p->x = fx;
        p->y = fy;
        p->px = fx;
        p->py = fy;
        p->vx = 0;
        p->vy = 0;
    }

    void draw() const override {
        return;
    }
};
