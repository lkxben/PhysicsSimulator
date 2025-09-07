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

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Physics Simulator"};
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Forcefield>> forcefields;
    ForceSystem fs;

    fs.addForce(Force::gravity(500));
    
    particles.emplace_back(std::make_unique<Particle>(400, 300, 0, 0, 10000.0, 10.0));
    particles.emplace_back(std::make_unique<Particle>(400, 200, 25, 0, 100.0, 5.0));
    particles.emplace_back(std::make_unique<Particle>(280, 250, 3, 22, 100.0, 5.0));

    SFMLRenderer renderer{window};
    Simulator simulator{obstacles, particles, forcefields, windowWidth, windowHeight};
    EventManager events{window};

    simulator.run(renderer, events, fs);

    return 0;
}