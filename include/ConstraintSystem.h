#pragma once
#include "Constraint.h"
#include <vector>

struct ConstraintSystem {
    std::vector<std::unique_ptr<Constraint>> constraints;

    void addConstraint(std::unique_ptr<Constraint> c) {
        constraints.push_back(std::move(c));
    }

    void applyAll(double dt) {
        for (int i = 0; i < 10; ++i) {
            for (auto& c : constraints) {
                c->apply(dt);
            }
        }
    }
};
