#pragma once
#include "System.h"

struct IntegratorSystem : public System {
    void update(World& world, double dt) override {
        for (auto& p : world.particles) {
            p->move(dt);
        }
    }
};