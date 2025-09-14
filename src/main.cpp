#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "../include/Renderer.h"
#include "../include/Simulator.h"
#include "../include/EventManager.h"
#include "../include/ForceSystem.h"
#include "../include/ConstraintSystem.h"
#include "../include/DistanceConstraint.h"
#include "../include/FixedPointConstraint.h"
#include "../include/MaxDistanceConstraint.h"

int main() {
    // Set up window
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 1000;

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Double Pendulum"};
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Forcefield>> forcefields;
    std::vector<std::unique_ptr<Constraint>> constraints;
    ForceSystem fs;
    ConstraintSystem cs;

    // Pivot particle (fixed)
    auto pivot = std::make_unique<Particle>(ParticleParams{
        .x = 400.0,
        .y = 500.0,
        .radius = 5.0,
        .color = sf::Color::Yellow
    });

    // First bob (middle)
    auto middle = std::make_unique<Particle>(ParticleParams{
        .x = 430.0,        
        .y = 400.0,
        .mass = 10.0,
        .radius = 15.0,
        .color = sf::Color::Red,
        .vx = 10.0
    });

    Particle* pivotPtr = pivot.get();
    Particle* middlePtr = middle.get();

    particles.push_back(std::move(pivot));
    particles.push_back(std::move(middle));

    auto c1 = std::make_unique<MaxDistanceConstraint>(pivotPtr, middlePtr, 100.0);
    cs.addConstraint(std::move(c1));
    constraints.push_back(std::make_unique<MaxDistanceConstraint>(pivotPtr, middlePtr, 100.0, sf::Color::White));

    auto c3 = std::make_unique<FixedPointConstraint>(pivotPtr, 400, 500);
    cs.addConstraint(std::move(c3));
    constraints.push_back(std::make_unique<FixedPointConstraint>(pivotPtr, 400, 500));

    forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(400, 300, 800, 600, sf::Color::Black),
        std::make_unique<GravityEffect>(50)
    ));

    SFMLRenderer renderer{window};
    Simulator simulator{obstacles, particles, forcefields, constraints, windowWidth, windowHeight};
    EventManager events{window};

    simulator.run(renderer, events, fs, cs);

    return 0;
}