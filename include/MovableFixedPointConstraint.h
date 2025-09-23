#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "FixedPointConstraint.h"
#include "Interactive.h"

struct MovableFixedPointConstraint : public FixedPointConstraint, public Interactive {
    bool isDragging = false;
    sf::Vector2f dragStart;
    sf::Vector2f dragCurrent;

    MovableFixedPointConstraint(Particle* particle, double x, double y)
        : FixedPointConstraint(particle, x, y) {}

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override {
        sf::Vector2f mousePos(
            static_cast<float>(sf::Mouse::getPosition(window).x),
            static_cast<float>(sf::Mouse::getPosition(window).y)
        );

        if (event.is<sf::Event::MouseButtonPressed>()) {
            if (!isDragging && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                float dx = mousePos.x - fx;
                float dy = mousePos.y - fy;
                if (dx*dx + dy*dy <= 100.f) { // grab radius ~10px
                    isDragging = true;
                    dragStart = mousePos;
                    dragCurrent = mousePos;
                }
            }
        }
        else if (event.is<sf::Event::MouseMoved>()) {
            if (isDragging) {
                dragCurrent = mousePos;
                fx = dragCurrent.x;
                fy = dragCurrent.y;
            }
        }
        else if (event.is<sf::Event::MouseButtonReleased>()) {
            if (isDragging) {
                isDragging = false;
            }
        }
    }

    void draw(sf::RenderWindow& window) const override {
        return;
    }
};
