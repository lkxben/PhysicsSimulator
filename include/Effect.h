#pragma once
#include <random>
#include <chrono>
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

        double dx = p.x - p.px;
        double dy = p.y - p.py;
        p.px = p.x - dx * (1.0 - k*dt);
        p.py = p.y - dy * (1.0 - k*dt);
    }
};

struct GravityEffect : Effect {
    double g;
    GravityEffect(double g_) : g(g_) {}
    void apply(Particle& p, double dt) const override {
        p.vy += g * dt;

        p.py -= g * dt * dt;
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

        p.px -= force * directionX * dt * dt;
        p.py -= force * directionY * dt * dt;
    }
};

struct FlagWindEffect : Effect {
    double directionX;
    double directionY;
    double minStrength;
    double maxStrength;
    mutable double currentStrength;
    mutable double targetStrength;
    mutable double changeSpeed;
    mutable std::mt19937 rng;
    mutable std::uniform_real_distribution<double> durationDist;
    mutable std::uniform_real_distribution<double> strengthDist;
    mutable double timer;
    mutable double nextSwitch;

    FlagWindEffect(double dirX, double dirY, double minS = 0.0, double maxS = 200.0)
        : directionX(dirX), directionY(dirY),
          minStrength(minS), maxStrength(maxS),
          currentStrength(minS), targetStrength(maxS),
          timer(0.0), nextSwitch(0.0),
          rng(std::random_device{}()),
          durationDist(0.3, 1.0),
          strengthDist(minS, maxS)
    {
        nextSwitch = durationDist(rng);
        targetStrength = strengthDist(rng);
        changeSpeed = (targetStrength - currentStrength) / nextSwitch;
    }

    void apply(Particle& p, double dt) const override {
        timer += dt;

        // smooth interpolation
        currentStrength += changeSpeed * dt;

        if ((changeSpeed > 0 && currentStrength >= targetStrength) ||
            (changeSpeed < 0 && currentStrength <= targetStrength) ||
            timer >= nextSwitch)
        {
            // new gust target
            timer = 0.0;
            nextSwitch = durationDist(rng);
            targetStrength = strengthDist(rng);
            changeSpeed = (targetStrength - currentStrength) / nextSwitch;
        }

        p.vx += currentStrength * directionX * dt;
        p.vy += currentStrength * directionY * dt;

        p.px -= currentStrength * directionX * dt * dt;
        p.py -= currentStrength * directionY * dt * dt;
    }
};