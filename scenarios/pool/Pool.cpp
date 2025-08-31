#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "../include/Renderer.h"
#include "../include/Simulator.h"
#include "CueBall.h"
#include "Pocket.h"
#include "PoolSimulator.h"

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

    for (size_t i = 0; i < std::size(balls); ++i) {
        double x = balls[i].x + posOffset(gen);
        double y = balls[i].y + posOffset(gen);
        sf::Color color = (i % 2 == 0) ? sf::Color(255, 165, 0) : sf::Color(128, 0, 128);
        particles.emplace_back(std::make_unique<CueBall>(x, y, 0.0, 0.0, 1.0, 10.0, 0.9, color));
    }

    particles.emplace_back(std::make_unique<DraggableParticle>(200, 300, 1.0, 10.0, 0.9, sf::Color::White));

    // Generate cushions
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(220.0, 108.0, 324.0, 24.0)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(580.0, 108.0, 324.0, 24.0)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(220.0, 492.0, 324.0, 24.0)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(580.0, 492.0, 324.0, 24.0));

    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(28.0, 219.0, 162.0, 24.0, M_PI / 2));  
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(28.0, 381.0, 162.0, 24.0, M_PI / 2));  
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(772.0, 219.0, 162.0, 24.0, M_PI / 2)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(772.0, 381.0, 162.0, 24.0, M_PI / 2));

    float pocketRadius = 16.0f;

    obstacles.push_back(std::make_unique<Pocket>(28.0 - 24.0/2.0, 108.0 - 24.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(400.0, 108.0 - 24.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(772.0 + 24.0/2.0, 108.0 - 24.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(28.0 - 24.0/2.0, 492.0 + 24.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(400.0, 492.0 + 24.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(772.0 + 24.0/2.0, 492.0 + 24.0/2.0, pocketRadius));

    // Generate forcefields
    forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(400, 300, 720, 360),
        std::make_unique<DragEffect>(0.25)
    ));

    SFMLRenderer renderer{window};
    PoolSimulator simulator{obstacles, particles, forcefields, windowWidth, windowHeight};
    
    simulator.run(renderer);

    return 0;
}