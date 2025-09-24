#pragma once
#include "Constraint.h"
#include "Particle.h"

struct ElasticDistanceConstraint : Constraint {
    Particle* a;
    Particle* b;
    double restLength;
    sf::Color color;

    ElasticDistanceConstraint(Particle* a_, Particle* b_, double length_, sf::Color color_ = sf::Color::White)
        : a(a_), b(b_), restLength(length_), color(color_) {}

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


void apply(double dt) override {
    double dx = b->x - a->x;
    double dy = b->y - a->y;
    double dist = std::sqrt(dx*dx + dy*dy);
    if (dist < 1e-6) return;

    // Hooke's law: F = -k * (dist - restLength)
    double stiffness = 50.0;  // spring constant, tweak for desired elasticity
    double damping   = 0.1;   // optional damping
    double forceMag = stiffness * (dist - restLength);

    double fx = forceMag * dx / dist;
    double fy = forceMag * dy / dist;

    double invMassA = (a->mass > 0) ? 1.0 / a->mass : 0.0;
    double invMassB = (b->mass > 0) ? 1.0 / b->mass : 0.0;

    // --- Euler: apply as acceleration/velocity change ---
    a->ax += fx * invMassA;
    a->ay += fy * invMassA;
    b->ax -= fx * invMassB;
    b->ay -= fy * invMassB;

    // --- Verlet: optional damping along relative velocity ---
    double relVx = (b->x - b->px)/dt - (a->x - a->px)/dt;
    double relVy = (b->y - b->py)/dt - (a->y - a->py)/dt;
    double dampingForceX = relVx * damping;
    double dampingForceY = relVy * damping;

    a->px += dampingForceX * invMassA * dt;
    a->py += dampingForceY * invMassA * dt;
    b->px -= dampingForceX * invMassB * dt;
    b->py -= dampingForceY * invMassB * dt;
}
};
