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
        particles.emplace_back(std::make_unique<Particle>(x, y, 0.0, 0.0, 1.0, 5.0));
    }
    particles.emplace_back(std::make_unique<Particle>(200, 400, 200, -92, 1.0, 5.0));

    // Generate obstacles
    obstacles.push_back(std::make_unique<SolidRectObstacle>(400.0, 300, 1.0, 200.0, 0.0, 1.3));
    // obstacles.push_back(std::make_unique<LineObstacle>(400.0, 150.0, 500.0, 0.0));
    // obstacles.push_back(std::make_unique<LineObstacle>(400.0, 450.0, 500.0, 0.0));
    // obstacles.push_back(std::make_unique<LineObstacle>(150.0, 300.0, 300.0, M_PI / 2));
    // obstacles.push_back(std::make_unique<LineObstacle>(650.0, 300.0, 300.0, M_PI / 2));
    // obstacles.push_back(std::make_unique<SolidCircleObstacle>(400, 300, 100.0));

    SFMLRenderer renderer{window};
    Simulator simulator{obstacles, particles, windowWidth, windowHeight};
    
    simulator.run(renderer);

    return 0;
}