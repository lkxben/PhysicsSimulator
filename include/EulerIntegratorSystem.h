#pragma once
#include "System.h"

struct EulerIntegratorSystem : public System {
    bool enforceBoundaries;
    double width;
    double height;

    EulerIntegratorSystem(bool enforce = false, double w = 0.0, double h = 0.0)
        : enforceBoundaries(enforce), width(w), height(h) {}

    void update(World& world, double dt) override {
        for (auto& p : world.particles) {
            p->vx += p->ax * dt;
            p->vy += p->ay * dt;
            p->x  += p->vx * dt;
            p->y  += p->vy * dt;
            if (!enforceBoundaries) continue;

            if (p->x - p->radius < 0.0) {
                p->vx = -p->vx * p->elasticity;
                p->x = p->radius;
            } else if (p->x + p->radius > width) {
                p->vx = -p->vx * p->elasticity;
                p->x = width - p->radius;
            }

            if (p->y - p->radius < 0.0) {
                p->vy = -p->vy * p->elasticity;
                p->y = p->radius;
            } else if (p->y + p->radius > height) {
                p->vy = -p->vy * p->elasticity;
                p->y = height - p->radius;
            }
        }
    }
};