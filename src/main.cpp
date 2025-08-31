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
        {410, 295}, {410, 305},    
        {420, 290}, {420, 300}, {420, 310},
        {430, 285}, {430, 295}, {430, 305}, {430, 315}
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> posOffset(-0.5, 0.5);

    for (auto& b : balls) {
        double x = b.x + posOffset(gen);
        double y = b.y + posOffset(gen);
        particles.emplace_back(std::make_unique<Particle>(x, y, 0.0, 0.0, 1.0, 5.0, 0.9));
    }

    particles.emplace_back(std::make_unique<DraggableParticle>(200, 300, 1.0, 5.0, 0.9));

    // Generate obstacles
    obstacles.push_back(std::make_unique<HollowRectObstacle>(400.0, 300.0, 500.0, 300.0, 10.0, 0, 0.9));

    // // Horizontal cushions (top & bottom)
    // obstacles.push_back(std::make_unique<SolidRectObstacle>(277.5, 150.0, 233.0, 7.0, 0.0, 0.9));
    // obstacles.push_back(std::make_unique<SolidRectObstacle>(522.5, 150.0, 233.0, 7.0, 0.0, 0.9));
    // obstacles.push_back(std::make_unique<SolidRectObstacle>(277.5, 450.0, 233.0, 7.0, 0.0, 0.9));
    // obstacles.push_back(std::make_unique<SolidRectObstacle>(522.5, 450.0, 233.0, 7.0, 0.0, 0.9));

    // // Vertical cushions (left & right)
    // obstacles.push_back(std::make_unique<SolidRectObstacle>(150.0, 225.0, 7.0, 135.0, 0.0, 0.9));
    // obstacles.push_back(std::make_unique<SolidRectObstacle>(150.0, 370.0, 7.0, 135.0, 0.0, 0.9));
    // obstacles.push_back(std::make_unique<SolidRectObstacle>(650.0, 225.0, 7.0, 135.0, 0.0, 0.9));
    // obstacles.push_back(std::make_unique<SolidRectObstacle>(650.0, 370.0, 7.0, 135.0, 0.0, 0.9));

    // Generate forcefields
    forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(400, 300, 480, 280),
        std::make_unique<DragEffect>(0.15)
    ));

    SFMLRenderer renderer{window};
    Simulator simulator{obstacles, particles, forcefields, windowWidth, windowHeight};
    
    simulator.run(renderer);

    return 0;
}