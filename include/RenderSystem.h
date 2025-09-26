#pragma once
#include "System.h"
#include "Particle.h"
#include "Obstacle.h"
#include "Forcefield.h"
#include "Constraint.h"
#include "World.h"
#include "raylib.h"

struct RenderSystem : public System {
    void update(World& world, double dt) override {
        BeginDrawing();
        ClearBackground(BLACK);

        for (const auto& f : world.forcefields)
            f->draw();

        for (const auto& o : world.obstacles)
            o->draw();

        for (const auto& p : world.particles)
            p->draw();

        for (const auto& c : world.constraints)
            c->draw();

        EndDrawing();
    }

    bool isRunning() const {
        return !WindowShouldClose();
    }
};