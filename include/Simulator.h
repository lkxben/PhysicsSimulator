#pragma once
#include <vector>
#include "Particle.h"
#include "Entity.h"
#include "RenderSystem.h"
#include "Obstacle.h"
#include "Forcefield.h"
#include "EventManager.h"
#include "ForceSystem.h"
#include "ConstraintSystem.h"
#include "Constraint.h"

struct Simulator {
    std::vector<std::unique_ptr<System>> systems;

    void addSystem(std::unique_ptr<System> system) {
        systems.push_back(std::move(system));
    }

    void run(World& world, EventManager& events) {
        sf::Clock clock;

        while (true) {
            events.pollEvents();
            double dt = clock.restart().asSeconds();

            for (auto& system : systems) {
                system->update(world, dt);
            }

            auto renderSystem = dynamic_cast<RenderSystem*>(systems.back().get());
            if (renderSystem && !renderSystem->isRunning()) break;
        }
    }

    virtual ~Simulator() = default;
};
