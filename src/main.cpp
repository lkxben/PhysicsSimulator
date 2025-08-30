#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "../include/Entity.h"
#include "../include/Particle.h"
#include "../include/Renderer.h"
#include "../include/Simulator.h"

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Physics Simulator"};

    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<Entity*> entities;
    int n = 10;
    double minV = -100.0, maxV = 100.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> xDist(0.0, windowWidth);
    std::uniform_real_distribution<double> yDist(0.0, windowHeight);
    std::uniform_real_distribution<double> vDist(minV, maxV);

    particles.emplace_back(std::make_unique<Particle>(400, 300, 100, -30, 50.0, 50.0));
    entities.push_back(particles.back().get());
    particles.emplace_back(std::make_unique<Particle>(200, 500, -30, -30, 50.0, 50.0));
    entities.push_back(particles.back().get());
    for (int i = 0; i < n; ++i) {
        double x = xDist(gen);
        double y = yDist(gen);
        double vx = vDist(gen);
        double vy = vDist(gen);
        particles.emplace_back(std::make_unique<Particle>(x, y, vx, vy, 1.0, 5.0));
        entities.push_back(particles.back().get());
    }

    Simulator simulator{entities, particles, windowWidth, windowHeight};
    SFMLRenderer renderer{window};

    sf::Clock clock;

    while (window.isOpen()) {
        while (auto eventOpt = window.pollEvent()) {
            sf::Event event = *eventOpt;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
        }

        double dt = clock.restart().asSeconds();
        simulator.update(dt);
        renderer.draw(entities);
    }

    return 0;
}