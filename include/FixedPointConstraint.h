#pragma once
#include "Constraint.h"
#include "Particle.h"

struct FixedPointConstraint : Constraint {
    Particle* p;
    double fx, fy;

    FixedPointConstraint(Particle* particle, double x, double y)
        : p(particle), fx(x), fy(y) {}

    void apply(double dt) override {
        p->x = fx;
        p->y = fy;
        p->vx = 0;
        p->vy = 0;
    }
};
