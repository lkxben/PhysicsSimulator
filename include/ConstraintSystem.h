#pragma once
#include "IntegratorType.h"
#include "System.h"
#include <vector>

struct ConstraintSystem : public System {
    IntegratorType integrator;

    ConstraintSystem(IntegratorType integrator_) : integrator(integrator_) {}

    void update(World& world, double dt) override {
        for (int i = 0; i < 10; ++i) {
            for (auto& c : world.constraints) {
                c->apply(dt, 10, integrator);
            }
        }
    }
};
