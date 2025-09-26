#pragma once
#include "System.h"

struct VerletIntegratorSystem : public System {
    bool enforceBoundaries;
    double width;
    double height;

    VerletIntegratorSystem(bool enforce = false, double w = 0.0, double h = 0.0)
        : enforceBoundaries(enforce), width(w), height(h) {}

    void update(World& world, double dt) override {
        for (auto& p : world.particles) {
            if (std::isnan(p->px)) {
                p->px = p->x - p->vx * dt;
                p->py = p->y - p->vy * dt;
            }

            double nx = p->x + (p->x - p->px) + p->ax * dt * dt;
            double ny = p->y + (p->y - p->py) + p->ay * dt * dt;
            p->px = p->x;
            p->py = p->y;
            p->x = nx;
            p->y = ny;

            if (!enforceBoundaries) continue;

            // X-axis boundaries
            if (p->x - p->radius < 0.0) {
                p->x = p->radius;
                p->px = p->x + (p->px - p->x) * -p->elasticity;
            } else if (p->x + p->radius > width) {
                p->x = width - p->radius;
                p->px = p->x + (p->px - p->x) * -p->elasticity;
            }

            // Y-axis boundaries
            if (p->y - p->radius < 0.0) {
                p->y = p->radius;
                p->py = p->y + (p->py - p->y) * -p->elasticity;
            } else if (p->y + p->radius > height) {
                p->y = height - p->radius;
                p->py = p->y + (p->py - p->y) * -p->elasticity;
            }
        }
    }
};