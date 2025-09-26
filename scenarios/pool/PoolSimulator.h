#pragma once
#include "../../include/Simulator.h"
#include "Pocket.h"
#include <iostream>

struct PoolSimulator : public Simulator {
    void run(World& world, EventManager& events) {
        Simulator::run(world, events);

        world.particles.erase(
            std::remove_if(world.particles.begin(), world.particles.end(),
                [](const std::unique_ptr<Particle>& p) {
                    if (Ball* cb = dynamic_cast<Ball*>(p.get()))
                        return !cb->alive;
                    return false;
                }),
            world.particles.end());
    }
};
