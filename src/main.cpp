#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "../include/Renderer.h"
#include "../include/Simulator.h"

int main() {
    // Set up window
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Physics Simulator"};
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Forcefield>> forcefields;

    // Generate particles
    struct Vec2 { double x, y; };
    Vec2 balls[] = {
        {400, 300},
        {416, 292}, {416, 308},
        {432, 284}, {432, 300}, {432, 316},
        {448, 276}, {448, 292}, {448, 308}, {448, 324}
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> posOffset(-0.5, 0.5);

    for (auto& b : balls) {
        double x = b.x + posOffset(gen);
        double y = b.y + posOffset(gen);
        particles.emplace_back(std::make_unique<Particle>(x, y, 0.0, 0.0, 1.0, 10.0, 0.9, sf::Color::Green));
    }

    particles.emplace_back(std::make_unique<LaunchableParticle>(200, 300, 1.0, 10.0, 0.9));

    // Generate obstacles
    obstacles.emplace_back(std::make_unique<HollowRectObstacle>(400, 300, 500, 300, 10.0, 0, 0.9, sf::Color::White));

    // Generate forcefields
    forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(400, 300, 480, 280),
        std::make_unique<DragEffect>(0.2)
    ));

    SFMLRenderer renderer{window};
    Simulator simulator{obstacles, particles, forcefields, windowWidth, windowHeight};
    
    simulator.run(renderer);

    return 0;
}