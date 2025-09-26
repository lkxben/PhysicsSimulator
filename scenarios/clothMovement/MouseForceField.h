#pragma once
#include "../include/Interactive.h"
#include "../include/Forcefield.h"
#include <SFML/Graphics.hpp>

struct MouseEffect : Effect {
    sf::Vector2f center;
    sf::Vector2f velocity;
    double strength;
    double radius;

    MouseEffect(double radius_, double strength_)
        : center(0.f, 0.f), velocity(0.f, 0.f), strength(strength_), radius(radius_) {}

    void apply(Particle& p, double dt) const override {
        double dx = p.x - center.x;
        double dy = p.y - center.y;
        double dist = std::sqrt(dx * dx + dy * dy);
        if (dist >= radius || p.mass <= 0) return;

        double falloff = 1.0 - (dist / radius);
        double fx = velocity.x * strength * falloff;
        double fy = velocity.y * strength * falloff;

        p.ax += fx / p.mass;
        p.ay += fy / p.mass;

        p.x += fx * dt / p.mass;
        p.y += fy * dt / p.mass;

        p.px += fx * dt * 0.01 / p.mass;
        p.py += fy * dt * 0.01 / p.mass;
    }

    void reset() {
        velocity = {0.f, 0.f};
    }
};

struct MovableCircleArea : public CircleArea {
    MovableCircleArea(float x_, float y_, float r_, sf::Color color_ = sf::Color::White)
        : CircleArea(x_, y_, r_, color_) {}

    void setPosition(float newX, float newY) { x = newX; y = newY; }
    sf::Vector2f getPosition() const { return {x, y}; }
};

struct MouseForcefield : public Interactive, public Forcefield {
    sf::Vector2f prevPos;
    bool firstFrame = true;

    MouseForcefield(double radius, double strength)
        : Forcefield(std::make_unique<MovableCircleArea>(0, 0, radius, sf::Color::Black),
                     std::make_unique<MouseEffect>(radius, strength)) {}

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, double dt) override {
        if (!event.is<sf::Event::MouseMoved>()) return;

        const auto* move = event.getIf<sf::Event::MouseMoved>();
        sf::Vector2f currPos = window.mapPixelToCoords({move->position.x, move->position.y});

        sf::Vector2f velocity;
        if (!firstFrame) {
            velocity = (currPos - prevPos) / static_cast<float>(dt);
        } else {
            velocity = {0.f, 0.f};
            firstFrame = false;
        }
        prevPos = currPos;

        if (auto* circle = dynamic_cast<MovableCircleArea*>(area.get()))
            circle->setPosition(currPos.x, currPos.y);

        if (auto* effect = dynamic_cast<MouseEffect*>(this->effect.get())) {
            effect->center = currPos;
            effect->velocity = velocity;
        }
    }

    void resetEffectVelocity() {
        if (auto* effect = dynamic_cast<MouseEffect*>(this->effect.get()))
            effect->reset();
    }
};