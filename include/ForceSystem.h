#pragma once
#include <vector>
#include "System.h"
#include "Particle.h"
#include "World.h"
#include "Force.h"

class ForceSystem : public System {
public:
    std::vector<Force> forces;

    void addForce(const Force& f) {
        forces.push_back(f);
    }

    void update(World& world, double dt) override {
        // Reset accelerations
        for (auto& p : world.particles) {
            p->ax = 0.0;
            p->ay = 0.0;
        }

        // Apply inter-particle forces
        for (auto& force : forces) {
            for (size_t i = 0; i < world.particles.size(); ++i) {
                for (size_t j = i + 1; j < world.particles.size(); ++j) {
                    force.func(*world.particles[i], *world.particles[j], dt);
                }
            }
        }
    }
};