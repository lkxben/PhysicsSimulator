#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "../include/Renderer.h"
#include "../include/Simulator.h"
#include "../include/EventManager.h"
#include "../include/ForceSystem.h"

int main() {
    // Set up window
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Three Body"};
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Forcefield>> forcefields;
    std::vector<std::unique_ptr<Constraint>> constraints;
    ForceSystem fs;

    fs.addForce(Force::gravity(2700));

    double cx = 400, cy = 300;
    double r = 100;
    double m = 500.0;
    double v = 70.0;

    particles.emplace_back(std::make_unique<Particle>(ParticleParams{
        .x = cx + r, .y = cy,
        .vx = 0.0, .vy = v,
        .mass = m,
        .radius = 5.0,
        .color = sf::Color::Red
    }));

    particles.emplace_back(std::make_unique<Particle>(ParticleParams{
        .x = cx - r * 0.5, .y = cy + r * std::sqrt(3) / 2,
        .vx = -v * std::sqrt(3) / 2, .vy = -v * 0.5,
        .mass = m,
        .radius = 5.0,
        .color = sf::Color::Blue
    }));

    particles.emplace_back(std::make_unique<Particle>(ParticleParams{
        .x = cx - r * 0.5, .y = cy - r * std::sqrt(3) / 2,
        .vx = v * std::sqrt(3) / 2, .vy = -v * 0.5,
        .mass = m,
        .radius = 5.0,
        .color = sf::Color::Green
    }));

    SFMLRenderer renderer{window};
    Simulator simulator{obstacles, particles, forcefields, constraints, windowWidth, windowHeight};
    EventManager events{window};
    ConstraintSystem cs;

    simulator.run(renderer, events, fs, cs);

    return 0;
}