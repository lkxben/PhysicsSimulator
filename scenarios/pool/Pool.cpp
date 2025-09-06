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
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Physics Simulator"};
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Forcefield>> forcefields;

    // Generate particles
    struct Vec2 { double x, y; };
    double xShift = 180;

    Vec2 balls[] = {
        {400 + xShift, 300},
        {416 + xShift, 292}, {416 + xShift, 308},
        {432 + xShift, 284}, {432 + xShift, 300}, {432 + xShift, 316},
        {448 + xShift, 276}, {448 + xShift, 292}, {448 + xShift, 308}, {448 + xShift, 324}
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

    particles.emplace_back(std::make_unique<LaunchableParticle>(200, 300, 1.0, 8.0, 0.9, sf::Color::White, 2000));

    // Generate cushions
    float cushionThickness = 8.0f;

    // Horizontal
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(212.0, 108.0, 328.0, cushionThickness)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(588.0, 108.0, 328.0, cushionThickness)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(212.0, 492.0, 328.0, cushionThickness)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(588.0, 492.0, 328.0, cushionThickness));

    // Vertical
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(28.0, 300.0, 344.0, cushionThickness, M_PI / 2)); 
    obstacles.emplace_back(std::make_unique<SolidRectObstacle>(772.0, 300.0, 344.0, cushionThickness, M_PI / 2));


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
    obstacles.push_back(makeTriangle(40, 104, 48, 112, 48, 104));
    obstacles.push_back(makeTriangle(376, 104, 376, 112, 384, 104));
    obstacles.push_back(makeTriangle(416, 104, 424, 112, 424, 104));
    obstacles.push_back(makeTriangle(752, 104, 752, 112, 760, 104));

    obstacles.push_back(makeTriangle(40, 496, 48, 496, 48, 488));
    obstacles.push_back(makeTriangle(376, 488, 376, 496, 384, 496));
    obstacles.push_back(makeTriangle(416, 496, 424, 496, 424, 488));
    obstacles.push_back(makeTriangle(752, 488, 752, 496, 760, 496));

    obstacles.push_back(makeTriangle(24, 120, 24, 128, 32, 128));
    obstacles.push_back(makeTriangle(24, 472, 24, 480, 32, 472));

    obstacles.push_back(makeTriangle(776, 120, 768, 128, 776, 128));
    obstacles.push_back(makeTriangle(768, 472, 776, 480, 776, 472));

    // Table
    obstacles.push_back(std::make_unique<HollowRectObstacle>(400, 300, 792, 432, 20.0));

    // Pockets
    float pocketRadius = 16.0f;

    obstacles.push_back(std::make_unique<Pocket>(24, 108.0 - 8.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(400.0, 108.0 - 8.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(772.0 + 8.0/2.0, 108.0 - 8.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(28.0 - 8.0/2.0, 492.0 + 8.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(400.0, 492.0 + 8.0/2.0, pocketRadius));
    obstacles.push_back(std::make_unique<Pocket>(772.0 + 8.0/2.0, 492.0 + 8.0/2.0, pocketRadius));

    // Generate forcefields
    forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(400, 300, 736, 376, sf::Color(0, 102, 204)),
        std::make_unique<DragEffect>(0.28)
    ));

    SFMLRenderer renderer{window};
    PoolSimulator simulator{obstacles, particles, forcefields, windowWidth, windowHeight};
    
    simulator.run(renderer);

    return 0;
}