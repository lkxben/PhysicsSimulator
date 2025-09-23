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

struct OscillatingForceEffect : Effect {
    double amplitude;
    double omega;
    double directionX;
    double directionY;
    mutable double elapsed = 0.0;

    OscillatingForceEffect(double amplitude_, double freqHz, double dirX, double dirY)
        : amplitude(amplitude_), omega(2.0 * M_PI * freqHz), directionX(dirX), directionY(dirY) {}

    void apply(Particle& p, double dt) const override {
        elapsed += dt;
        double force = amplitude * std::sin(omega * elapsed);
        p.vx += force * directionX * dt;
        p.vy += force * directionY * dt;
    }
};
