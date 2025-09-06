#include <SFML/Graphics.hpp>
#include <cmath>
#include "Particle.h"

struct LaunchableParticle : Particle {
    bool isDragging = false;
    sf::Vector2f dragStart;
    sf::Vector2f dragCurrent;

    LaunchableParticle(double x_, double y_, double mass_ = 1.0, float radius_ = 5.0, double elasticity_ = 1.0, sf::Color color_ = sf::Color::White)
        : Particle(x_, y_, 0.0, 0.0, mass_, radius_, elasticity_, color_) {}

    void handleEvent(const sf::RenderWindow& window) {
        const double EPS = 1e-1;
        if (std::abs(vx) < EPS) vx = 0;
        if (std::abs(vy) < EPS) vy = 0;
        if (vx != 0 || vy != 0) return;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));

            float dx = mousePos.x - static_cast<float>(x);
            float dy = mousePos.y - static_cast<float>(y);

            if (!isDragging && dx*dx + dy*dy <= radius*radius) {
                isDragging = true;
                dragStart = mousePos;
                dragCurrent = mousePos;
            } else if (isDragging) {
                dragCurrent = mousePos;
            }
        } else if (isDragging) {
            vx = static_cast<double>((dragStart.x - dragCurrent.x) * 2.0f);
            vy = static_cast<double>((dragStart.y - dragCurrent.y) * 2.0f);
            isDragging = false;
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
                float scale = 0.5f;
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
