#include <vector>
#include <memory>
#include <cmath>
#include "raylib.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

#include "../include/World.h"
#include "../include/Simulator.h"
#include "../include/EventManager.h"
#include "../include/ForceSystem.h"
#include "../include/RenderSystem.h"
#include "../include/EulerIntegratorSystem.h"
#include "../include/CollisionSystem.h"

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
    InitWindow(windowWidth, windowHeight, "Three Body Simulation");

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

void initSim()
{
    double cx = windowWidth / 2.0;
    double cy = windowHeight / 2.0;
    double r = 100;
    double m = 500.0;
    double v = 70.0;

    world.particles.push_back(std::make_unique<Particle>(ParticleParams{
        .x = cx + r, .y = cy,
        .vx = 0.0, .vy = v,
        .mass = m,
        .radius = 5.0,
        .color = RED
    }));

    world.particles.push_back(std::make_unique<Particle>(ParticleParams{
        .x = cx - r * 0.5, .y = cy + r * std::sqrt(3) / 2,
        .vx = -v * std::sqrt(3) / 2, .vy = -v * 0.5,
        .mass = m,
        .radius = 5.0,
        .color = BLUE
    }));

    world.particles.push_back(std::make_unique<Particle>(ParticleParams{
        .x = cx - r * 0.5, .y = cy - r * std::sqrt(3) / 2,
        .vx = v * std::sqrt(3) / 2, .vy = -v * 0.5,
        .mass = m,
        .radius = 5.0,
        .color = GREEN
    }));

    auto forceSystem = std::make_unique<ForceSystem>();
    forceSystem->addForce(Force::gravity(2700));

    simulator.addSystem(std::move(forceSystem));
    simulator.addSystem(std::make_unique<EulerIntegratorSystem>(true, windowWidth, windowHeight));
    simulator.addSystem(std::make_unique<CollisionSystem>(world, windowWidth, windowHeight));
    simulator.addSystem(std::make_unique<RenderSystem>());
}

void updateDrawFrame()
{
    simulator.step(world, events, dt);
}