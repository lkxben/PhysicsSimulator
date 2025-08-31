#pragma once
#include "Particle.h"

struct Effect {
    virtual void apply(Particle& p, double dt) const = 0;
    virtual ~Effect() = default;
};

struct DragEffect : Effect {
    double k;
    DragEffect(double k_) : k(k_) {}
    void apply(Particle& p, double dt) const override {
        p.vx -= p.vx * k * dt;
        p.vy -= p.vy * k * dt;
    }
};

struct GravityEffect : Effect {
    double g;
    GravityEffect(double g_) : g(g_) {}
    void apply(Particle& p, double dt) const override {
        p.vy += g * dt;
    }
};
