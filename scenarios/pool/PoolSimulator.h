#pragma once
#include "../../include/Simulator.h"
#include "Pocket.h"
#include <iostream>

struct PoolSimulator : public Simulator {
    PoolSimulator(std::vector<std::unique_ptr<Obstacle>>& obstacles_,
                  std::vector<std::unique_ptr<Particle>>& particles_,
                  std::vector<std::unique_ptr<Forcefield>>& forcefields_,
                  std::vector<std::unique_ptr<Constraint>>& constraints_,
                  double w, double h)
        : Simulator(obstacles_, particles_, forcefields_, constraints_, w, h)
    {}

    void update(double dt) {
        Simulator::update(dt);

        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                [](const std::unique_ptr<Particle>& p) {
                    if (Ball* cb = dynamic_cast<Ball*>(p.get()))
                        return !cb->alive;
                    return false;
                }),
            particles.end());
    }
};
