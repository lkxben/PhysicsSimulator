#pragma once
#include "../include/System.h"
#include "../include/World.h"
#include "../include/Interactive.h"
#include "../include/Constraint.h"
#include <SFML/Graphics.hpp>
#include <cmath>

class ClothCuttingSystem : public System, public Interactive {
public:
    sf::Vector2f center;
    float radius;

    ClothCuttingSystem(float radius_) : radius(radius_) {}

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, double dt) override {
        if (!event.is<sf::Event::MouseMoved>()) return;
        const auto* move = event.getIf<sf::Event::MouseMoved>();
        center = window.mapPixelToCoords({move->position.x, move->position.y});
    }

    void update(World& world, double dt) override {
        for (auto it = world.constraints.begin(); it != world.constraints.end();) {
            auto particles = (*it)->getParticles();
            if (particles.size() < 2) { ++it; continue; }

            bool remove = false;
            for (auto* p : particles) {
                float dist = std::hypot(p->x - center.x, p->y - center.y);
                if (dist <= radius) { remove = true; break; }
            }

            if (remove)
                it = world.constraints.erase(it);
            else
                ++it;
        }
    }
};