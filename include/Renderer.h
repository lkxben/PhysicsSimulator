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
#include "LaunchableParticle.h"
#include "Forcefield.h"
#include "Constraint.h"

class Renderer {
public:
    virtual void draw(const std::vector<std::unique_ptr<Particle>>& particles, const std::vector<std::unique_ptr<Obstacle>>& obstacless, const std::vector<std::unique_ptr<Forcefield>>& forcefields, const std::vector<std::unique_ptr<Constraint>>& constraints) = 0;
    virtual bool isRunning() const = 0;
};

class SFMLRenderer : public Renderer {
    sf::RenderWindow& window;
public:
    SFMLRenderer(sf::RenderWindow& w) : window(w) {}

    void draw(const std::vector<std::unique_ptr<Particle>>& particles, const std::vector<std::unique_ptr<Obstacle>>& obstacles, const std::vector<std::unique_ptr<Forcefield>>& forcefields, const std::vector<std::unique_ptr<Constraint>>& constraints) override {
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

        for (const auto& c : constraints) {
            c->draw(window);
        }

        window.display();
    }

    bool isRunning() const override {
        return window.isOpen();
    }
};
