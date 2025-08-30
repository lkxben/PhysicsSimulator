#pragma once
#include <vector>
#include "Particle.h"
#include <SFML/Graphics.hpp>

class Renderer {
public:
    virtual void draw(const std::vector<Particle>& particles) = 0;
};

class SFMLRenderer : public Renderer {
    sf::RenderWindow& window;
public:
    SFMLRenderer(sf::RenderWindow& w) : window(w) {}

    void draw(const std::vector<Particle>& particles) override {
        window.clear();
        for (const auto& p : particles) {
            sf::CircleShape shape{p.radius, 30}; 
            shape.setPosition({static_cast<float>(p.x - p.radius), static_cast<float>(p.y - p.radius)});
            window.draw(shape);
        }
        window.display();
    }
};
