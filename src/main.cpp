#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../include/World.h"
#include "../include/Simulator.h"
#include "../include/EventManager.h"
#include "../include/ForceSystem.h"
#include "../include/ForcefieldSystem.h"
#include "../include/ConstraintSystem.h"
#include "../include/MaxDistanceConstraint.h"
#include "../include/FixedPointConstraint.h"
#include "../include/MovableFixedPointConstraint.h"
#include "../include/ElasticDistanceConstraint.h"
#include "../include/RenderSystem.h"
#include "../include/EulerIntegratorSystem.h"
#include "../include/VerletIntegratorSystem.h"

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Cloth"};

    World world;

    const int rows = 20;
    const int cols = 20;
    const double spacing = 15.0;

    std::vector<std::vector<Particle*>> grid(rows, std::vector<Particle*>(cols, nullptr));

    // Create grid of particles
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            auto p = std::make_unique<Particle>(ParticleParams{
                .x = 200.0 + j * spacing,
                .y = 50.0 + i * spacing,
                .mass = 1.0,
                .radius = 1.0,
                .color = sf::Color::White
            });
            grid[i][j] = p.get();
            world.particles.push_back(std::move(p));
        }
    }

    // Structural & shear constraints
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Particle* p = grid[i][j];

            if (j < cols - 1)
                world.constraints.push_back(std::make_unique<MaxDistanceConstraint>(p, grid[i][j+1], spacing));

            if (i < rows - 1)
                world.constraints.push_back(std::make_unique<MaxDistanceConstraint>(p, grid[i+1][j], spacing));

            // if (i < rows - 1 && j < cols - 1) {
            //     world.constraints.push_back(std::make_unique<ElasticDistanceConstraint>(p, grid[i+1][j+1], spacing * 1.414));
            //     world.constraints.push_back(std::make_unique<ElasticDistanceConstraint>(grid[i+1][j], grid[i][j+1], spacing * 1.414));
            // }
        }
    }

    // Forcefields
    world.forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(windowWidth / 2, windowHeight / 2, windowWidth, windowHeight, sf::Color::Black),
        std::make_unique<GravityEffect>(100)
    ));

    world.forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(windowWidth / 2, windowHeight / 2, windowWidth, windowHeight, sf::Color::Black),
        std::make_unique<DragEffect>(1)
    ));

    // Systems
    Simulator simulator;
    simulator.addSystem(std::make_unique<VerletIntegratorSystem>());
    // simulator.addSystem(std::make_unique<EulerIntegratorSystem>());
    simulator.addSystem(std::make_unique<ForcefieldSystem>());
    simulator.addSystem(std::make_unique<ConstraintSystem>(IntegratorType::Verlet));
    // simulator.addSystem(std::make_unique<ConstraintSystem>(IntegratorType::Euler));
    simulator.addSystem(std::make_unique<RenderSystem>(window));

    EventManager events{window};
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

    simulator.run(world, events);

    return 0;
}