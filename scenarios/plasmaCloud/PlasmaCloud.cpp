#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
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

    InitWindow(windowWidth, windowHeight, "Plasma Cloud");
    SetTargetFPS(60);

    World world;

    // Random generators
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> distX(100.0, 700.0);
    std::uniform_real_distribution<double> distY(100.0, 500.0);
    std::uniform_real_distribution<double> distV(-10.0, 10.0);

    int numParticles = 100;
    for (int i = 0; i < numParticles; i++) {
        bool isProton = (i % 3 == 0);
        double charge = isProton ? +5.0 : -5.0;
        double mass   = isProton ? 20.0 : 1.0;
        Color color = isProton ? RED : BLUE;

        world.particles.push_back(std::make_unique<Particle>(ParticleParams{
            .x = distX(rng),
            .y = distY(rng),
            .vx = distV(rng),
            .vy = distV(rng),
            .mass = mass,
            .radius = isProton ? 4.0f : 2.0f,
            .charge = charge,
            .color = color
        }));
    }

    // Systems
    auto forceSystem = std::make_unique<ForceSystem>();
    forceSystem->addForce(Force::electric(1000));

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