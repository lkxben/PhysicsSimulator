#pragma once
#include <vector>
#include "Particle.h"
#include "Force.h"

class ForceSystem {
public:
    std::vector<Force> forces;

    void addForce(const Force& f) {
        forces.push_back(f);
    }

    void apply(const std::vector<std::unique_ptr<Particle>>& particles, double dt);
};

