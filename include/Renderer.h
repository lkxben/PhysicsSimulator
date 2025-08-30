#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include <vector>
#include "Particle.h"
#include "SolidRectObstacle.h"
#include "HollowRectObstacle.h"
#include "SolidCircleObstacle.h"

class Renderer {
public:
    virtual void draw(const std::vector<std::unique_ptr<Particle>>& particles, const std::vector<std::unique_ptr<Obstacle>>& obstacless) = 0;
    virtual bool isRunning() const = 0;
    virtual void pollEvents() = 0; 
};

class SFMLRenderer : public Renderer {
    sf::RenderWindow& window;
public:
    SFMLRenderer(sf::RenderWindow& w) : window(w) {}

    void draw(const std::vector<std::unique_ptr<Particle>>& particles, const std::vector<std::unique_ptr<Obstacle>>& obstacles) override {
        window.clear();
        for (const auto& p : particles) {
            p->draw(window);
        }

        for (const auto& o : obstacles) {
            o->draw(window);
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
