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
    particles.emplace_back(std::make_unique<Particle>(350, 250, 0, 0, 1.0, 5.0));
    particles.emplace_back(std::make_unique<Particle>(210, 310, 200, 0, 1.0, 5.0));

    // Generate obstacles
    // obstacles.push_back(std::make_unique<HollowRectObstacle>(400.0, 300.0, 500.0, 300.0, 10.0, 0, 1.1));
    // obstacles.push_back(std::make_unique<HollowCircleObstacle>(400.0, 300.0, 200.0, 2.0, 1.1));
    // obstacles.push_back(std::make_unique<SolidRectObstacle>(400.0, 300.0, 200.0, 200.0, 0, 1.1));
std::vector<sf::Vector2f> crescentVertices;
double centerX = 400, centerY = 300;
double outerRadius = 120, innerRadius = 80;
int outerPoints = 25;
int innerPoints = 25;

// Outer arc (top to bottom, clockwise)
for (int i = 0; i < outerPoints; ++i) {
    double angle = M_PI * (i / static_cast<double>(outerPoints - 1)); // 180° arc
    double x = centerX + outerRadius * cos(angle);
    double y = centerY - outerRadius * sin(angle);
    crescentVertices.push_back({static_cast<float>(x), static_cast<float>(y)});
}

// Inner arc (bottom to top, clockwise) - offset to create crescent
for (int i = innerPoints - 1; i >= 0; --i) {
    double angle = M_PI * (i / static_cast<double>(innerPoints - 1)); // 180° arc
    double x = centerX + innerRadius * cos(angle) + 40; // offset right to make crescent
    double y = centerY - innerRadius * sin(angle);
    crescentVertices.push_back({static_cast<float>(x), static_cast<float>(y)});
}
    
    // obstacles.push_back(std::make_unique<HollowPolygonObstacle>(crescentVertices, 0.9));
    obstacles.push_back(std::make_unique<SolidPolygonObstacle>(crescentVertices, 0.9));
    

    SFMLRenderer renderer{window};
    Simulator simulator{obstacles, particles, windowWidth, windowHeight};
    
    simulator.run(renderer);

    return 0;
}