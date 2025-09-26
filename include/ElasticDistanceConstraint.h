#pragma once
#include "Constraint.h"
#include "Particle.h"
#include <vector>
#include <cmath>

struct ElasticDistanceConstraint : Constraint {
    Particle* a;
    Particle* b;
    double restLength;
    double maxLength;
    double k;
    Color color;

    ElasticDistanceConstraint(Particle* a_, Particle* b_, double restLength_, double maxLength_, double k_ = 0.01, Color color_ = WHITE)
        : a(a_), b(b_), restLength(restLength_), maxLength(maxLength_), k(k_), color(color_) {}

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

        DrawLine(static_cast<int>(startX), static_cast<int>(startY),
                 static_cast<int>(endX), static_cast<int>(endY), color);
    }

    void apply(double dt, int iterations, IntegratorType integrator) override {
        double dx = b->x - a->x;
        double dy = b->y - a->y;
        double dist = std::sqrt(dx*dx + dy*dy);
        if (dist < 1e-6) return;

        double invMassA = (a->mass > 0 ? 1.0 / a->mass : 0.0);
        double invMassB = (b->mass > 0 ? 1.0 / b->mass : 0.0);
        double invMassSum = invMassA + invMassB;
        if (invMassSum == 0) return;

        if (integrator == IntegratorType::Euler) {
            double forceMag = k * (dist - restLength);
            double fx = forceMag * dx / dist;
            double fy = forceMag * dy / dist;

            a->ax += fx * invMassA;
            a->ay += fy * invMassA;
            b->ax -= fx * invMassB;
            b->ay -= fy * invMassB;

            if (dist > maxLength) {
                double diff = (dist - maxLength) / dist;
                double corrX = diff * dx;
                double corrY = diff * dy;

                a->x += corrX * invMassA / invMassSum;
                a->y += corrY * invMassA / invMassSum;
                b->x -= corrX * invMassB / invMassSum;
                b->y -= corrY * invMassB / invMassSum;
            }

        } else if (integrator == IntegratorType::Verlet) {
            double k_iter = k / iterations;
            double diff = (dist - restLength) / dist * k_iter;

            double corrX = diff * dx;
            double corrY = diff * dy;

            a->x += corrX * invMassA / invMassSum;
            a->y += corrY * invMassA / invMassSum;
            b->x -= corrX * invMassB / invMassSum;
            b->y -= corrY * invMassB / invMassSum;

            if (invMassA > 0) {
                a->px += corrX * 0.05;
                a->py += corrY * 0.05;
            }
            if (invMassB > 0) {
                b->px -= corrX * 0.05;
                b->py -= corrY * 0.05;
            }

            if (dist > maxLength) {
                double diffMax = (dist - maxLength) / dist;
                double corrXMax = diffMax * dx;
                double corrYMax = diffMax * dy;

                a->x += corrXMax * invMassA / invMassSum;
                a->y += corrYMax * invMassA / invMassSum;
                b->x -= corrXMax * invMassB / invMassSum;
                b->y -= corrYMax * invMassB / invMassSum;
            }
        }
    }
};