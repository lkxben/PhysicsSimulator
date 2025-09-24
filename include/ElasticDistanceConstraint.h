#pragma once
#include "Constraint.h"
#include "Particle.h"

struct ElasticDistanceConstraint : Constraint {
    Particle* a;
    Particle* b;
    double restLength;
    double k;
    sf::Color color;

    ElasticDistanceConstraint(Particle* a_, Particle* b_, double length_, double k_ = 0.05, sf::Color color_ = sf::Color::White)
        : a(a_), b(b_), restLength(length_), k(k_), color(color_) {}

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
    if (dist < 1e-6) return;

    double invMassA = (a->mass > 0 ? 1.0 / a->mass : 0.0);
    double invMassB = (b->mass > 0 ? 1.0 / b->mass : 0.0);
    if (invMassA + invMassB == 0) return;

    // Spring force
    double forceMag = k * (dist - restLength);
    double fx = forceMag * dx / dist;
    double fy = forceMag * dy / dist;

    // Apply accelerations
    a->ax += fx * invMassA;
    a->ay += fy * invMassA;
    b->ax -= fx * invMassB;
    b->ay -= fy * invMassB;

    // Update velocities
    a->vx += a->ax * dt;
    a->vy += a->ay * dt;
    b->vx += b->ax * dt;
    b->vy += b->ay * dt;

    // Optionally reset acceleration for next frame
    a->ax = 0;
    a->ay = 0;
    b->ax = 0;
    b->ay = 0;
}
};
