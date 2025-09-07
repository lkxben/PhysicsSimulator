#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "../include/Renderer.h"
#include "../include/Simulator.h"
#include "../include/EventManager.h"

int main() {
    // Set up window
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Physics Simulator"};
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Forcefield>> forcefields;

    EventManager events{window};
    auto particle = std::make_unique<LaunchableParticle>(200, 300, 1.0, 10.0, 0.9);
    events.registerInteractive(particle.get());
    particles.push_back(std::move(particle));

    SFMLRenderer renderer{window};
    Simulator simulator{obstacles, particles, forcefields, windowWidth, windowHeight};

    simulator.run(renderer, events);

    return 0;
}