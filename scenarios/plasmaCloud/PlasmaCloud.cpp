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

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Plasma Cloud"};
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Forcefield>> forcefields;
    ForceSystem fs;

    fs.addForce(Force::electric(1000));

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> distX(100.0, 700.0);
    std::uniform_real_distribution<double> distY(100.0, 500.0);
    std::uniform_real_distribution<double> distV(-10.0, 10.0); // small random kick

    int numParticles = 100;

    for (int i = 0; i < numParticles; i++) {
        bool isProton = (i % 3 == 0);           // alternate protons/electrons
        double charge = isProton ? +5.0 : -5.0;
        double mass   = isProton ? 20.0 : 1.0;  // heavy proton, light electron
        sf::Color color = isProton ? sf::Color::Red : sf::Color::Blue;

        particles.emplace_back(std::make_unique<Particle>(ParticleParams{
            .x = distX(rng),
            .y = distY(rng),
            .vx = distV(rng),
            .vy = distV(rng),
            .mass = mass,
            .radius = isProton ? 4.0f : 2.0f,   // visual size
            .charge = charge,
            .color = color
        }));
    }

    SFMLRenderer renderer{window};
    Simulator simulator{obstacles, particles, forcefields, windowWidth, windowHeight};
    EventManager events{window};
    ConstraintSystem cs;

    simulator.run(renderer, events, fs, cs);

    return 0;
}