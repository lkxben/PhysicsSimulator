#include <vector>
#include <memory>
#include <cmath>
#include "../include/World.h"
#include "../include/Simulator.h"
#include "../include/EventManager.h"
#include "../include/ForceSystem.h"
#include "../include/ForcefieldSystem.h"
#include "../include/ConstraintSystem.h"
#include "../include/CollisionSystem.h"
#include "../include/RenderSystem.h"
#include "../include/EulerIntegratorSystem.h"

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    InitWindow(windowWidth, windowHeight, "Three Body");
    SetTargetFPS(120);

    World world;

    // Set up particles
    double cx = 400, cy = 300;
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

    // Create systems
    auto forceSystem = std::make_unique<ForceSystem>();
    forceSystem->addForce(Force::gravity(2700));

    // Set up simulator
    Simulator simulator;
    simulator.addSystem(std::move(forceSystem));
    simulator.addSystem(std::make_unique<EulerIntegratorSystem>(true, windowWidth, windowHeight));
    simulator.addSystem(std::make_unique<CollisionSystem>(world, windowWidth, windowHeight));
    simulator.addSystem(std::make_unique<RenderSystem>());

    EventManager events;

    // Run simulation
    simulator.run(world, events);

    return 0;
}