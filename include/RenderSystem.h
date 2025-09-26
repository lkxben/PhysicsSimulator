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
#include "System.h"

struct RenderSystem : public System {
    sf::RenderWindow& window;

    RenderSystem(sf::RenderWindow& w) : window(w) {}

    void update(World& world, double dt) override {
        window.clear();

        for (const auto& f : world.forcefields)
            f->draw(window);

        for (const auto& o : world.obstacles)
            o->draw(window);

        for (const auto& p : world.particles)
            p->draw(window);

        for (const auto& c : world.constraints)
            c->draw(window);

        window.display();
    }

    bool isRunning() const {
        return window.isOpen();
    }
};