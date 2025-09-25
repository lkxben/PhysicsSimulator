#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../include/World.h"
#include "../include/Simulator.h"
#include "../include/EventManager.h"
#include "../include/ForceSystem.h"
#include "../include/ForcefieldSystem.h"
#include "../include/ConstraintSystem.h"
#include "../include/DistanceConstraint.h"
#include "../include/FixedPointConstraint.h"
#include "../include/RenderSystem.h"
#include "../include/EulerIntegratorSystem.h"

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 1000;

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Double Pendulum"};
    World world;

    // Pivot particle (fixed)
    auto pivot = std::make_unique<Particle>(ParticleParams{
        .x = 400.0, .y = 500.0, .radius = 5.0, .color = sf::Color::Yellow
    });

    // First bob (middle)
    auto middle = std::make_unique<Particle>(ParticleParams{
        .x = 400.0, .y = 400.0, .mass = 50.0, .radius = 15.0, .color = sf::Color::Red
    });

    // Second bob (end)
    auto end = std::make_unique<Particle>(ParticleParams{
        .x = 400.0, .y = 250.0, .mass = 20.0, .radius = 10.0, .color = sf::Color::Blue, .vx = 200.0
    });

    Particle* pivotPtr = pivot.get();
    Particle* middlePtr = middle.get();
    Particle* endPtr = end.get();

    world.particles.push_back(std::move(pivot));
    world.particles.push_back(std::move(middle));
    world.particles.push_back(std::move(end));

    // Add constraints directly to world
    world.constraints.push_back(std::make_unique<DistanceConstraint>(pivotPtr, middlePtr, 100.0));
    world.constraints.push_back(std::make_unique<DistanceConstraint>(middlePtr, endPtr, 150.0));
    world.constraints.push_back(std::make_unique<FixedPointConstraint>(pivotPtr, 400, 500));

    // Forcefield (gravity)
    world.forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(windowWidth/2, windowHeight/2, windowWidth, windowHeight, sf::Color::Black),
        std::make_unique<GravityEffect>(50)
    ));

    // Simulator and systems
    Simulator simulator;
    simulator.addSystem(std::make_unique<ForcefieldSystem>());
    simulator.addSystem(std::make_unique<EulerIntegratorSystem>(true, windowWidth, windowHeight));
    simulator.addSystem(std::make_unique<ConstraintSystem>(IntegratorType::Euler));
    simulator.addSystem(std::make_unique<RenderSystem>(window));

    EventManager events{window};

    // Run simulation
    simulator.run(world, events);

    return 0;
}