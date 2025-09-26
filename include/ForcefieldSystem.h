#pragma once
#include "System.h"

struct ForcefieldSystem : public System {
    void update(World& world, double dt) override {
        for (auto& ff : world.forcefields) {
            for (auto& p : world.particles) {
                ff->apply(*p, dt);
            }
        }
    }
};