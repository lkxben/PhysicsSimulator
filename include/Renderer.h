#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include <vector>
#include "Particle.h"
#include "SolidRectObstacle.h"
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
            sf::CircleShape shape{p->radius, 30};
            shape.setPosition({static_cast<float>(p->x - p->radius),
                            static_cast<float>(p->y - p->radius)});
            window.draw(shape);
        }

        for (const auto& o : obstacles) {
            if (auto rect = dynamic_cast<SolidRectObstacle*>(o.get())) {
                sf::RectangleShape shape{sf::Vector2f(static_cast<float>(rect->width), static_cast<float>(rect->height))};
                shape.setOrigin({static_cast<float>(rect->width)/2, static_cast<float>(rect->height)/2});
                shape.setPosition({static_cast<float>(rect->x), static_cast<float>(rect->y)});
                shape.setRotation(sf::degrees(static_cast<float>(rect->rotation * 180.0 / M_PI)));
                window.draw(shape);
            } else if (auto circle = dynamic_cast<SolidCircleObstacle*>(o.get())) {
                sf::CircleShape shape{static_cast<float>(circle->radius), 30};
                shape.setPosition({static_cast<float>(circle->x - circle->radius),
                                static_cast<float>(circle->y - circle->radius)});
                window.draw(shape);
            }
        }
        window.display();
    }

    void draw(const std::vector<Obstacle>& obstacles) {
        
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
