#include <SFML/Graphics.hpp>
#include <cmath>
#include "Particle.h"

struct DraggableParticle : Particle {
    bool isDragging = false;
    sf::Vector2f dragStart;

    DraggableParticle(double x_, double y_, double mass_ = 1.0, float radius_ = 5.0, double elasticity_ = 1.0)
        : Particle(x_, y_, mass_, 0.0, 0.0, radius_, elasticity_) {}

    void handleEvent(const sf::RenderWindow& window) {
        if (vx != 0 || vy != 0) return;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));
            float dx = mousePos.x - static_cast<float>(x);
            float dy = mousePos.y - static_cast<float>(y);
            if (!isDragging && dx*dx + dy*dy <= radius*radius) {
                isDragging = true;
                dragStart = mousePos;
            }
        } else if (isDragging) {
            sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));
            vx = static_cast<double>((dragStart.x - mousePos.x) * 2.0f);
            vy = static_cast<double>((dragStart.y - mousePos.y) * 2.0f);
            isDragging = false;
        }
    }

    void draw(sf::RenderWindow& window) const override {
        sf::CircleShape shape(radius);
        shape.setOrigin(sf::Vector2f(radius, radius));
        shape.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        shape.setFillColor(sf::Color::Red);
        window.draw(shape);

        if (isDragging) {
            sf::Vertex line[2];
            line[0].position = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
            line[0].color = sf::Color::Yellow;
            line[1].position = dragStart;
            line[1].color = sf::Color::Yellow;

            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
    }
};
