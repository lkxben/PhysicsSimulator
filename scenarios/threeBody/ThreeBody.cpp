#include <SFML/Graphics.hpp>
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

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Three Body"};

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
        .color = sf::Color::Red
    }));

    world.particles.push_back(std::make_unique<Particle>(ParticleParams{
        .x = cx - r * 0.5, .y = cy + r * std::sqrt(3) / 2,
        .vx = -v * std::sqrt(3) / 2, .vy = -v * 0.5,
        .mass = m,
        .radius = 5.0,
        .color = sf::Color::Blue
    }));

    world.particles.push_back(std::make_unique<Particle>(ParticleParams{
        .x = cx - r * 0.5, .y = cy - r * std::sqrt(3) / 2,
        .vx = v * std::sqrt(3) / 2, .vy = -v * 0.5,
        .mass = m,
        .radius = 5.0,
        .color = sf::Color::Green
    }));

    // Create systems
    auto forceSystem = std::make_unique<ForceSystem>();
    forceSystem->addForce(Force::gravity(2700));

    // Set up simulator
    Simulator simulator;
    simulator.addSystem(std::move(forceSystem));
    simulator.addSystem(std::make_unique<EulerIntegratorSystem>(true, windowWidth, windowHeight));
    simulator.addSystem(std::make_unique<CollisionSystem>(world, windowWidth, windowHeight));
    simulator.addSystem(std::make_unique<RenderSystem>(window));

    EventManager events{window};

    // Run simulation
    simulator.run(world, events);

    return 0;
}