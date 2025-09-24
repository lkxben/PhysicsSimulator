#pragma once
#include "Constraint.h"
#include "Particle.h"

struct MaxDistanceConstraint : Constraint {
    Particle* a;
    Particle* b;
    double maxLength;
    sf::Color color;

    MaxDistanceConstraint(Particle* a_, Particle* b_, double maxLength_, sf::Color color_ = sf::Color::White)
        : a(a_), b(b_), maxLength(maxLength_), color(color_) {}

    void draw(sf::RenderWindow& window) const override {
        float dx = static_cast<float>(b->x - a->x);
        float dy = static_cast<float>(b->y - a->y);
        float dist = std::sqrt(dx*dx + dy*dy);
        if (dist < 1e-6f) return;

        float ux = dx / dist;
        float uy = dy / dist;

        sf::Vector2f start(static_cast<float>(a->x) + ux * a->radius,
                        static_cast<float>(a->y) + uy * a->radius);
        sf::Vector2f end(static_cast<float>(b->x) - ux * b->radius,
                        static_cast<float>(b->y) - uy * b->radius);

        sf::VertexArray line(sf::PrimitiveType::Lines, 2);
        line[0].position = start;
        line[0].color = color;
        line[1].position = end;
        line[1].color = color;

        window.draw(line);
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

            double dot = relVx * dx + relVy * dy;
            double k = dot / (dist * dist);

            double vxCorrection = k * dx;
            double vyCorrection = k * dy;

            a->vx += vxCorrection * invMassA / invMassSum;
            a->vy += vyCorrection * invMassA / invMassSum;
            b->vx -= vxCorrection * invMassB / invMassSum;
            b->vy -= vyCorrection * invMassB / invMassSum;

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
