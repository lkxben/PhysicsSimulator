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
        particles.emplace_back(std::make_unique<Particle>(x, y, 0.0, 0.0, 1.0, 5.0, 0.8));
    }
    particles.emplace_back(std::make_unique<Particle>(10, 100, 300, 30, 1.0, 5.0));
    particles.emplace_back(std::make_unique<Particle>(210, 300, 200, 0, 1.0, 5.0));

    // Generate obstacles
    // obstacles.push_back(std::make_unique<HollowRectObstacle>(400.0, 300.0, 500.0, 300.0, 10.0, 0, 0.8));
    obstacles.push_back(std::make_unique<HollowCircleObstacle>(400.0, 300.0, 200.0, 2.0, 0.9));

    SFMLRenderer renderer{window};
    Simulator simulator{obstacles, particles, windowWidth, windowHeight};
    
    simulator.run(renderer);

    return 0;
}