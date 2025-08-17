#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "../include/Particle.h"
#include "../include/ParticleRenderer.h"
#include "../include/ParticleSimulator.h"

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window{sf::VideoMode{sf::Vector2u{windowWidth, windowHeight}}, "Particle Simulator"};

    std::vector<Particle> particles;
    int n = 1000;
    double minV = -100.0, maxV = 100.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> xDist(0.0, windowWidth);
    std::uniform_real_distribution<double> yDist(0.0, windowHeight);
    std::uniform_real_distribution<double> vDist(minV, maxV);

    for (int i = 0; i < n; ++i) {
        double x = xDist(gen);
        double y = yDist(gen);
        double vx = vDist(gen);
        double vy = vDist(gen);
        particles.emplace_back(x, y, vx, vy, 3.0);
    }

    ParticleSimulator simulator{particles, windowWidth, windowHeight};
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

        renderer.draw(particles);
    }

    return 0;
}