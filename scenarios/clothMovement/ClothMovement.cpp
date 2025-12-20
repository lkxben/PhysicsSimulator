#include <vector>
#include <memory>
#include "raylib.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

#include "../include/World.h"
#include "../include/Simulator.h"
#include "../include/EventManager.h"
#include "../include/ForceSystem.h"
#include "../include/ForcefieldSystem.h"
#include "../include/ConstraintSystem.h"
#include "../include/MovableFixedPointConstraint.h"
#include "../include/ElasticDistanceConstraint.h"
#include "../include/RenderSystem.h"
#include "../include/VerletIntegratorSystem.h"
#include "MouseForceField.h"

const unsigned int windowWidth = 800;
const unsigned int windowHeight = 600;
const double FPS = 60.0;
const double dt = 1.0 / FPS;

World world;
Simulator simulator;
EventManager events;

void updateDrawFrame();
void initSim();

int main() {
    InitWindow(windowWidth, windowHeight, "Cloth Movement");

    initSim();

#if defined(__EMSCRIPTEN__)
    emscripten_set_main_loop(updateDrawFrame, FPS, 1);
#else
    SetTargetFPS(FPS);
    simulator.run(world, events);
#endif

    CloseWindow();
    return 0;
}

void initSim() {
    const int rows = 30;
    const int cols = 50;
    const double spacing = 8.0;

    std::vector<std::vector<Particle*>> grid(rows, std::vector<Particle*>(cols, nullptr));

    double startX = (windowWidth - (cols - 1) * spacing) / 2.0;
    double startY = 50.0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            auto p = std::make_unique<Particle>(ParticleParams{
                .x = startX + j * spacing,
                .y = startY + i * spacing,
                .mass = 1.0,
                .radius = 0.0,
                .color = WHITE
            });
            grid[i][j] = p.get();
            world.particles.push_back(std::move(p));
        }
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Particle* p = grid[i][j];

            if (j < cols - 1)
                world.constraints.push_back(std::make_unique<ElasticDistanceConstraint>(p, grid[i][j+1], spacing, spacing + 1.0));

            if (i < rows - 1)
                world.constraints.push_back(std::make_unique<ElasticDistanceConstraint>(p, grid[i+1][j], spacing, spacing + 1.0));
        }
    }

    // Forcefields
    world.forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(windowWidth / 2, windowHeight / 2, windowWidth, windowHeight, BLACK),
        std::make_unique<GravityEffect>(200)
    ));

    world.forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(windowWidth / 2, windowHeight / 2, windowWidth, windowHeight, BLACK),
        std::make_unique<DragEffect>(1)
    ));

    std::vector<int> fixedIndices = {0, cols/3, 2*cols/3, cols - 1};
    for (int idx : fixedIndices) {
        auto movable = std::make_unique<MovableFixedPointConstraint>(
            grid[0][idx], grid[0][idx]->x, grid[0][idx]->y
        );
        events.registerInteractive(movable.get());
        world.constraints.push_back(std::move(movable));
    }

    {
        auto movable = std::make_unique<MovableFixedPointConstraint>(
            grid[0][0], grid[0][0]->x, grid[0][0]->y
        );
        events.registerInteractive(movable.get());
        world.constraints.push_back(std::move(movable));
    }
    {
        auto movable = std::make_unique<MovableFixedPointConstraint>(
            grid[0][cols - 1], grid[0][cols - 1]->x, grid[0][cols - 1]->y
        );
        events.registerInteractive(movable.get());
        world.constraints.push_back(std::move(movable));
    }
    auto mouseForceField = std::make_unique<MouseForcefield>(10.0, 3.5);
    events.registerInteractive(mouseForceField.get());
    world.forcefields.push_back(std::move(mouseForceField));

    // Systems
    simulator.addSystem(std::make_unique<ForcefieldSystem>());
    simulator.addSystem(std::make_unique<VerletIntegratorSystem>());

    simulator.addSystem(std::make_unique<ConstraintSystem>(IntegratorType::Verlet));

    simulator.addSystem(std::make_unique<RenderSystem>());
}

void updateDrawFrame() {
    simulator.step(world, events, dt);
}