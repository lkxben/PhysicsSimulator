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
#include "raylib.h"

struct Simulator {
    std::vector<std::unique_ptr<System>> systems;

    void addSystem(std::unique_ptr<System> system) {
        systems.push_back(std::move(system));
    }

    void run(World& world, EventManager& events) {
        double lastTime = GetTime();

        while (!WindowShouldClose()) {
            double currentTime = GetTime();
            double dt = currentTime - lastTime;
            lastTime = currentTime;

            events.pollEvents(dt);

            world.reset();
            for (auto& system : systems) {
                system->update(world, dt);
            }
        }
    }

    void step(World& world, EventManager& events, double dt) {
        events.pollEvents(dt);

        world.reset();
        for (auto& system : systems) {
            system->update(world, dt);
        }
    }

    virtual ~Simulator() = default;
};
