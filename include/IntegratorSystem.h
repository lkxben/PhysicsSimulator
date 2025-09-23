#pragma once
#include "System.h"

struct IntegratorSystem : public System {
    bool enforceBoundaries;
    double width;
    double height;

    IntegratorSystem(bool enforce = false, double w = 0.0, double h = 0.0)
        : enforceBoundaries(enforce), width(w), height(h) {}

    void update(World& world, double dt) override {
        for (auto& p : world.particles) {
            p->move(dt);
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