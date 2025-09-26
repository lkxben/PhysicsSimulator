#pragma once
#include <cmath>
#include <vector>
#include "Constraint.h"
#include "Particle.h"
#include "raylib.h"

struct DistanceConstraint : Constraint {
    Particle* a;
    Particle* b;
    double restLength;
    Color color;

    DistanceConstraint(Particle* a_, Particle* b_, double length_, Color color_ = WHITE)
        : a(a_), b(b_), restLength(length_), color(color_) {}

    std::vector<Particle*> getParticles() const override {
        return {a, b};
    }

    void draw() const override {
        float dx = static_cast<float>(b->x - a->x);
        float dy = static_cast<float>(b->y - a->y);
        float dist = std::sqrt(dx*dx + dy*dy);
        if (dist < 1e-6f) return;

        float ux = dx / dist;
        float uy = dy / dist;

        float startX = static_cast<float>(a->x + ux * a->radius);
        float startY = static_cast<float>(a->y + uy * a->radius);
        float endX = static_cast<float>(b->x - ux * b->radius);
        float endY = static_cast<float>(b->y - uy * b->radius);

        DrawLineEx({startX, startY}, {endX, endY}, 1.0f, CLITERAL(Color){color.r, color.g, color.b, color.a});
    }

    void apply(double dt, int iterations, IntegratorType integrator) override {
        double dx = b->x - a->x;
        double dy = b->y - a->y;
        double dist = std::sqrt(dx*dx + dy*dy);
        if (dist < 1e-6) return;

        double diff = (dist - restLength) / dist;

        double invMassA = (a->mass > 0) ? 1.0 / a->mass : 0.0;
        double invMassB = (b->mass > 0) ? 1.0 / b->mass : 0.0;
        double invMassSum = invMassA + invMassB;
        if (invMassSum == 0) return;

        double correctionX = diff * dx;
        double correctionY = diff * dy;
        a->x += correctionX * (invMassA / invMassSum);
        a->y += correctionY * (invMassA / invMassSum);
        b->x -= correctionX * (invMassB / invMassSum);
        b->y -= correctionY * (invMassB / invMassSum);

        double relVx = b->vx - a->vx;
        double relVy = b->vy - a->vy;

        double dot = relVx * dx + relVy * dy;
        double k = dot / (dist * dist);

        double vxCorrection = k * dx;
        double vyCorrection = k * dy;

        a->vx += vxCorrection * invMassA / invMassSum;
        a->vy += vyCorrection * invMassA / invMassSum;
        b->vx -= vxCorrection * invMassB / invMassSum;
        b->vy -= vyCorrection * invMassB / invMassSum;
    }
};