#pragma once
#include "Constraint.h"
#include "Particle.h"

struct DistanceConstraint : Constraint {
    Particle* a;
    Particle* b;
    double restLength;

    DistanceConstraint(Particle* a_, Particle* b_, double length_)
        : a(a_), b(b_), restLength(length_) {}

void apply(double dt) override {
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
