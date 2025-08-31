#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include <vector>
#include "Particle.h"
#include "SolidRectObstacle.h"
#include "HollowRectObstacle.h"
#include "SolidCircleObstacle.h"
#include "HollowCircleObstacle.h"
#include "SolidPolygonObstacle.h"
#include "HollowPolygonObstacle.h"
#include "DraggableParticle.h"
#include "Forcefield.h"

class Renderer {
public:
    virtual void draw(const std::vector<std::unique_ptr<Particle>>& particles, const std::vector<std::unique_ptr<Obstacle>>& obstacless, const std::vector<std::unique_ptr<Forcefield>>& forcefields) = 0;
    virtual bool isRunning() const = 0;
    virtual void pollEvents(std::vector<std::unique_ptr<Particle>>& particles) = 0; 
};

class SFMLRenderer : public Renderer {
    sf::RenderWindow& window;
public:
    SFMLRenderer(sf::RenderWindow& w) : window(w) {}

    void draw(const std::vector<std::unique_ptr<Particle>>& particles, const std::vector<std::unique_ptr<Obstacle>>& obstacles, const std::vector<std::unique_ptr<Forcefield>>& forcefields) override {
        window.clear();

        for (const auto& f : forcefields) {
            f->draw(window);
        }

        for (const auto& o : obstacles) {
            o->draw(window);
        }

        for (const auto& p : particles) {
            p->draw(window);
        }
        window.display();
    }

    bool isRunning() const override {
        return window.isOpen();
    }

    void pollEvents(std::vector<std::unique_ptr<Particle>>& particles) override {
        auto eventOpt = window.pollEvent();
        sf::Event event = *eventOpt; 
        if (event.is<sf::Event::Closed>()) { window.close(); }
        for (auto& p : particles) {
            if (auto dp = dynamic_cast<DraggableParticle*>(p.get())) {
                dp->handleEvent(window);
            }
        }
    }
};
