#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"

struct Area {
    sf::Color color;

    Area(sf::Color color_ = sf::Color::White) : color(color_) {};

    virtual bool contains(const Particle& p) const = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual ~Area() = default;
};

struct CircleArea : Area {
    float x, y, radius;
    CircleArea(float x_, float y_, float r_, sf::Color color_ = sf::Color::White) : Area(color_), x(x_), y(y_), radius(r_) {}

    bool contains(const Particle& p) const override {
        double dx = static_cast<double>(p.x) - static_cast<double>(x);
        double dy = static_cast<double>(p.y) - static_cast<double>(y);
        return dx * dx + dy * dy <= static_cast<double>(radius) * radius;
    }

    void draw(sf::RenderWindow& window) const override {
        sf::CircleShape circle(radius);
        circle.setOrigin({radius, radius});
        circle.setPosition({x, y});
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(color);
        circle.setOutlineThickness(1.f);
        window.draw(circle);
    }
};

struct RectArea : Area {
    float x, y, width, height;
    RectArea(float x_, float y_, float w_, float h_, sf::Color color_ = sf::Color::White) : Area(color_), x(x_), y(y_), width(w_), height(h_) {}

    bool contains(const Particle& p) const override {
        double halfW = static_cast<double>(width) / 2.0;
        double halfH = static_cast<double>(height) / 2.0;
        double dx = p.x - static_cast<double>(x);
        double dy = p.y - static_cast<double>(y);
        return dx >= -halfW && dx <= halfW &&
               dy >= -halfH && dy <= halfH;
    }

    void draw(sf::RenderWindow& window) const override {
        sf::RectangleShape rect(sf::Vector2f(width, height));
        rect.setOrigin({width / 2.f, height / 2.f}); 
        rect.setPosition({x, y});
        rect.setFillColor(color);
        window.draw(rect);
    }
};
