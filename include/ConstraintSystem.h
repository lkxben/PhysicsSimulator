#pragma once
#include "System.h"
#include <vector>

struct ConstraintSystem : public System {
    void update(World& world, double dt) override {
        for (int i = 0; i < 10; ++i) {
            for (auto& c : world.constraints) {
                c->apply(dt);
            }
        }
    }
};
