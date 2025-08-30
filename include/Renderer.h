#pragma once
#include <vector>
#include "Particle.h"
#include <SFML/Graphics.hpp>

class Renderer {
public:
    virtual void draw(const std::vector<Entity*>& entities) = 0;
    virtual bool isRunning() const = 0;
    virtual void pollEvents() = 0; 
};

class SFMLRenderer : public Renderer {
    sf::RenderWindow& window;
public:
    SFMLRenderer(sf::RenderWindow& w) : window(w) {}

    void draw(const std::vector<Entity*>& entities) override {
        window.clear();
        for (auto e : entities) {
            if (auto p = dynamic_cast<Particle*>(e)) {
                sf::CircleShape shape{p->radius, 30};
                shape.setPosition({static_cast<float>(p->x - p->radius), static_cast<float>(p->y - p->radius)});
                window.draw(shape);
            }
        }
        window.display();
    }

    bool isRunning() const override {
        return window.isOpen();
    }

    void pollEvents() override {
        auto eventOpt = window.pollEvent();
        sf::Event event = *eventOpt; 
        if (event.is<sf::Event::Closed>()) { window.close(); }
    }
};
