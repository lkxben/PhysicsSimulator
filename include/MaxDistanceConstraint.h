#pragma once
#include "Constraint.h"
#include "Particle.h"
#include <cmath>

struct MaxDistanceConstraint : Constraint {
    Particle* a;
    Particle* b;
    double maxLength;
    Color color;

    MaxDistanceConstraint(Particle* a_, Particle* b_, double maxLength_, Color color_ = WHITE)
        : a(a_), b(b_), maxLength(maxLength_), color(color_) {}

    std::vector<Particle*> getParticles() const override {
        return {a, b};
    }

    void draw() const override {
        double dx = b->x - a->x;
        double dy = b->y - a->y;
        double dist = std::sqrt(dx*dx + dy*dy);
        if (dist < 1e-6 || dist <= maxLength) return;

        double ux = dx / dist;
        double uy = dy / dist;

        Vector2 start{static_cast<float>(a->x + ux * a->radius),
                    static_cast<float>(a->y + uy * a->radius)};
        Vector2 end{static_cast<float>(b->x - ux * b->radius),
                    static_cast<float>(b->y - uy * b->radius)};

        DrawLineEx(start, end, 1.0f, color);
    }

    void apply(double dt, int iterations, IntegratorType integrator) override {
        double dx = b->x - a->x;
        double dy = b->y - a->y;
        double dist = std::sqrt(dx*dx + dy*dy);
        if (dist < 1e-6 || dist <= maxLength) return;

        double invMassA = (a->mass > 0 ? 1.0 / a->mass : 0.0);
        double invMassB = (b->mass > 0 ? 1.0 / b->mass : 0.0);
        double invMassSum = invMassA + invMassB;
        if (invMassSum == 0) return;

        if (integrator == IntegratorType::Euler) {
            double diff = (dist - maxLength) / dist;
            double correctionX = diff * dx;
            double correctionY = diff * dy;

            a->x += correctionX * (invMassA / invMassSum);
            a->y += correctionY * (invMassA / invMassSum);
            b->x -= correctionX * (invMassB / invMassSum);
            b->y -= correctionY * (invMassB / invMassSum);

            double relVx = b->vx - a->vx;
            double relVy = b->vy - a->vy;

            double k = (relVx * dx + relVy * dy) / (dist * dist);

            a->vx += k * dx * invMassA / invMassSum;
            a->vy += k * dy * invMassA / invMassSum;
            b->vx -= k * dx * invMassB / invMassSum;
            b->vy -= k * dy * invMassB / invMassSum;

        } else if (integrator == IntegratorType::Verlet) {
            double k_iter = 1.0 / iterations;
            double diff = (dist - maxLength) / dist * k_iter;

            double correctionX = diff * dx;
            double correctionY = diff * dy;

            a->x += correctionX * (invMassA / invMassSum);
            a->y += correctionY * (invMassA / invMassSum);
            b->x -= correctionX * (invMassB / invMassSum);
            b->y -= correctionY * (invMassB / invMassSum);

            if (invMassA > 0) {
                a->px += correctionX * 0.05;
                a->py += correctionY * 0.05;
            }
            if (invMassB > 0) {
                b->px -= correctionX * 0.05;
                b->py -= correctionY * 0.05;
            }
        }
    }
};