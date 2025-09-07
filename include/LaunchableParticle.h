#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Particle.h"
#include "Interactive.h"

struct LaunchableParticle : public Particle, public Interactive {
    bool isDragging = false;
    sf::Vector2f dragStart;
    sf::Vector2f dragCurrent;
    double maxForce;
    double forceScale;

    LaunchableParticle(double x_, double y_, double mass_ = 1.0, float radius_ = 5.0, double elasticity_ = 1.0, sf::Color color_ = sf::Color::White, double maxForce_ = 500.0f, double forceScale_ = 4.0f)
        : Particle(x_, y_, 0.0, 0.0, mass_, radius_, elasticity_, color_), maxForce(maxForce_), forceScale(forceScale_) {}

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override {
        const double EPS = 1e-1;
        if (std::abs(vx) < EPS) vx = 0;
        if (std::abs(vy) < EPS) vy = 0;
        if (vx != 0 || vy != 0) return;

        sf::Vector2f mousePos(
            static_cast<float>(sf::Mouse::getPosition(window).x),
            static_cast<float>(sf::Mouse::getPosition(window).y)
        );

        if (event.is<sf::Event::MouseButtonPressed>()) {
            if (!isDragging && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                float dx = mousePos.x - x;
                float dy = mousePos.y - y;
                if (dx*dx + dy*dy <= radius*radius) {
                    isDragging = true;
                    dragStart = mousePos;
                    dragCurrent = mousePos;
                }
            }
        }
        else if (event.is<sf::Event::MouseMoved>()) {
            if (isDragging) {
                dragCurrent = mousePos;
            }
        }
        else if (event.is<sf::Event::MouseButtonReleased>()) {
            if (isDragging) {
                sf::Vector2f pull = dragStart - dragCurrent;
                float length = std::sqrt(pull.x * pull.x + pull.y * pull.y);
                if (length > 0.f) {
                    sf::Vector2f dir = pull / length;
                    float forceMag = length * forceScale;
                    if (forceMag > maxForce) forceMag = maxForce;

                    vx = static_cast<double>(dir.x * forceMag / mass);
                    vy = static_cast<double>(dir.y * forceMag / mass);
                }
                isDragging = false;
            }
        }
    }

    void draw(sf::RenderWindow& window) const override {
        sf::CircleShape shape(radius);
        shape.setOrigin(sf::Vector2f(radius, radius));
        shape.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        shape.setFillColor(color);
        window.draw(shape);

        if (isDragging) {
            sf::Vector2f center(static_cast<float>(x), static_cast<float>(y));
            sf::Vector2f toMouse = dragCurrent - center;
            float dist = std::sqrt(toMouse.x * toMouse.x + toMouse.y * toMouse.y);

            if (dist > 0) {
                sf::Vector2f dir = toMouse / dist;
                float scale = 1.0f;
                sf::Vector2f lineEnd = center - dir * (radius + dist * scale);

                sf::Vertex line[2];
                line[0].position = center;
                line[0].color = sf::Color::Yellow;
                line[1].position = lineEnd;
                line[1].color = sf::Color::Yellow;

                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }
    }
};
