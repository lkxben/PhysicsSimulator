#pragma once
#include <cstddef>
#include "Obstacle.h"

struct SolidRectObstacle : public Obstacle {
    double width;
    double height;
    double rotation;

    SolidRectObstacle(double x_, double y_, double width_, double height_, double rotation_ = 0.0, double elasticity_ = 1.0)
        : Obstacle(x_, y_, elasticity_), width(width_), height(height_), rotation(rotation_) {}
        
    virtual ~SolidRectObstacle() = default;

    void draw(sf::RenderWindow& window) const override {
        sf::RectangleShape shape{sf::Vector2f(static_cast<float>(width), static_cast<float>(height))};
        shape.setOrigin({static_cast<float>(width)/2, static_cast<float>(height)/2});
        shape.setPosition({static_cast<float>(x), static_cast<float>(y)});
        shape.setRotation(sf::degrees(static_cast<float>(rotation * 180.0 / M_PI)));
        window.draw(shape);
    }
};
