#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "../include/Renderer.h"
#include "../include/Simulator.h"
#include "Ball.h"
#include "Pocket.h"
#include "PoolSimulator.h"

int main() {
    // Set up window
    const unsigned int windowWidth = 1000;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Pool"};
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Forcefield>> forcefields;

    // Generate particles
    struct Vec2 { double x, y; };
    double xShift = 180;

    Vec2 balls[] = {
        {500 + xShift, 300},
        {516 + xShift, 292}, {516 + xShift, 308},
        {532 + xShift, 284}, {532 + xShift, 300}, {532 + xShift, 316},
        {548 + xShift, 276}, {548 + xShift, 292}, {548 + xShift, 308}, {548 + xShift, 324}
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> posOffset(-0.7, 0.7);

    for (size_t i = 0; i < std::size(balls); ++i) {
        double x = balls[i].x + posOffset(gen);
        double y = balls[i].y + posOffset(gen);
        sf::Color color = (i % 2 == 0) ? sf::Color(255, 140, 0) : sf::Color(0, 255, 255);
        particles.emplace_back(std::make_unique<Ball>(x, y, 0.0, 0.0, 1.0, 8.0, 0.9, color));
    }

    // Generate cushions
    float cushionThickness = 8.0f;

    // Horizontal
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(312.0, 108.0, 328.0, cushionThickness)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(688.0, 108.0, 328.0, cushionThickness)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(312.0, 492.0, 328.0, cushionThickness)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(688.0, 492.0, 328.0, cushionThickness));

    // Vertical
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(128.0, 300.0, 344.0, cushionThickness, M_PI / 2)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(872.0, 300.0, 344.0, cushionThickness, M_PI / 2));


    // Pocket Jaws
    auto makeTriangle = [](float x1, float y1, float x2, float y2, float tipX, float tipY) {
        return std::make_unique<SolidPolygonObstacle>(
            std::vector<sf::Vector2f>{
                {x1, y1},
                {x2, y2},
                {tipX, tipY}
            },
            0.9,
            sf::Color::White
        );
    };
    obstacles.push_back(makeTriangle(140, 104, 148, 112, 148, 104));
    obstacles.push_back(makeTriangle(476, 104, 476, 112, 484, 104));
    obstacles.push_back(makeTriangle(516, 104, 524, 112, 524, 104));
    obstacles.push_back(makeTriangle(852, 104, 852, 112, 860, 104));

    obstacles.push_back(makeTriangle(140, 496, 148, 496, 148, 488));
    obstacles.push_back(makeTriangle(476, 488, 476, 496, 484, 496));
    obstacles.push_back(makeTriangle(516, 496, 524, 496, 524, 488));
    obstacles.push_back(makeTriangle(852, 488, 852, 496, 860, 496));

    obstacles.push_back(makeTriangle(124, 120, 124, 128, 132, 128));
    obstacles.push_back(makeTriangle(124, 472, 124, 480, 132, 472));

    obstacles.push_back(makeTriangle(876, 120, 868, 128, 876, 128));
    obstacles.push_back(makeTriangle(868, 472, 876, 480, 876, 472));

    // Table
    obstacles.push_back(std::make_unique<HollowRectObstacle>(500, 300, 792, 432, 20.0));

    // Pockets
    float pocketRadius = 16.0f;

    obstacles.push_back(std::make_unique<Pocket>(124, 108.0 - 8.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(500.0, 108.0 - 8.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(872.0 + 8.0/2.0, 108.0 - 8.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(128.0 - 8.0/2.0, 492.0 + 8.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(500.0, 492.0 + 8.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(872.0 + 8.0/2.0, 492.0 + 8.0/2.0, pocketRadius));

    // Generate forcefields
    forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(500, 300, 736, 376, sf::Color(0, 102, 204)),
        std::make_unique<DragEffect>(0.28)
    ));

    EventManager events{window};
    auto particle = std::make_unique<LaunchableParticle>(300, 300, 1.0, 8.0, 0.9, sf::Color::White, 2000);
    events.registerInteractive(particle.get());
    particles.push_back(std::move(particle));

    SFMLRenderer renderer{window};
    ForceSystem fs;
    PoolSimulator simulator{obstacles, particles, forcefields, windowWidth, windowHeight};
    
    simulator.run(renderer, events, fs);

    return 0;
}