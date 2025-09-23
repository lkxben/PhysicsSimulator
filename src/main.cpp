#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../include/Renderer.h"
#include "../include/Simulator.h"
#include "../include/EventManager.h"
#include "../include/ForceSystem.h"
#include "../include/ConstraintSystem.h"
#include "../include/MaxDistanceConstraint.h"
#include "../include/FixedPointConstraint.h"

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Cloth"};

    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Forcefield>> forcefields;
    std::vector<std::unique_ptr<Constraint>> constraints;
    ForceSystem fs;
    ConstraintSystem cs;

    const int rows = 20;
    const int cols = 20;
    const double spacing = 15.0;

    // Create grid of particles
    std::vector<std::vector<Particle*>> grid(rows, std::vector<Particle*>(cols, nullptr));

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
            particles.push_back(std::move(p));
        }
    }

    // Add structural (horizontal & vertical) max-distance constraints
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Particle* p = grid[i][j];

            if (j < cols - 1) { // horizontal
                auto c = std::make_unique<MaxDistanceConstraint>(p, grid[i][j+1], spacing);
                cs.addConstraint(std::move(c));
                constraints.push_back(std::make_unique<MaxDistanceConstraint>(p, grid[i][j+1], spacing));
            }

            if (i < rows - 1) { // vertical
                auto c = std::make_unique<MaxDistanceConstraint>(p, grid[i+1][j], spacing);
                cs.addConstraint(std::move(c));
                constraints.push_back(std::make_unique<MaxDistanceConstraint>(p, grid[i+1][j], spacing));
            }

            // Diagonal shear constraints
            if (i < rows - 1 && j < cols - 1) {
                auto c1 = std::make_unique<MaxDistanceConstraint>(p, grid[i+1][j+1], spacing * 1.414); // diagonal
                cs.addConstraint(std::move(c1));
                constraints.push_back(std::make_unique<MaxDistanceConstraint>(p, grid[i+1][j+1], spacing * 1.414));

                auto c2 = std::make_unique<MaxDistanceConstraint>(grid[i+1][j], grid[i][j+1], spacing * 1.414);
                cs.addConstraint(std::move(c2));
                constraints.push_back(std::make_unique<MaxDistanceConstraint>(grid[i+1][j], grid[i][j+1], spacing * 1.414));
            }
        }
    }

    cs.addConstraint(std::make_unique<FixedPointConstraint>(grid[0][0], grid[0][0]->x, grid[0][0]->y));
    constraints.push_back(std::make_unique<FixedPointConstraint>(grid[0][0], grid[0][0]->x, grid[0][0]->y));

    cs.addConstraint(std::make_unique<FixedPointConstraint>(grid[rows-1][0], grid[rows-1][0]->x, grid[rows-1][0]->y - 50));
    constraints.push_back(std::make_unique<FixedPointConstraint>(grid[rows-1][0], grid[rows-1][0]->x, grid[rows-1][0]->y - 50));

    // Gravity
    // forcefields.push_back(std::make_unique<Forcefield>(
    //     std::make_unique<RectArea>(0, 0, windowWidth, windowHeight, sf::Color::Black),
    //     std::make_unique<GravityEffect>(20.0)
    // ));

    forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(0, 0, windowWidth, windowHeight, sf::Color::Black),
        std::make_unique<OscillatingForceEffect>(0.1, 1.0, 500.0, 0)
    ));

    SFMLRenderer renderer{window};
    Simulator simulator{obstacles, particles, forcefields, constraints, windowWidth, windowHeight};
    EventManager events{window};

    simulator.run(renderer, events, fs, cs);

    return 0;
}