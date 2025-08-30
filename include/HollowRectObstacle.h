#pragma once
#include <cstddef>
#include "Obstacle.h"

struct HollowRectObstacle : public Obstacle {
    double width;
    double height;
    double thickness;
    double rotation;

    HollowRectObstacle(double x_, double y_, double width_, double height_, double thickness_ = 1.0, double rotation_ = 0.0, double elasticity_ = 1.0)
        : Obstacle(x_, y_, elasticity_), width(width_), height(height_), thickness(thickness_), rotation(rotation_) {}
        
    virtual ~HollowRectObstacle() = default;

    void draw(sf::RenderWindow& window) const override {
        float halfW = static_cast<float>(width / 2.0);
        float halfH = static_cast<float>(height / 2.0);
        float halfT = static_cast<float>(thickness / 2.0);
        sf::Vector2f center{static_cast<float>(x), static_cast<float>(y)};
        float rotDeg = static_cast<float>(rotation * 180.0 / M_PI);

        sf::RectangleShape topWall{sf::Vector2f(static_cast<float>(width), static_cast<float>(thickness))};
        topWall.setOrigin(topWall.getSize() / 2.0f);
        topWall.setPosition(center + sf::Vector2f(0.f, -halfH + halfT));
        topWall.setRotation(sf::degrees(rotDeg));
        window.draw(topWall);

        sf::RectangleShape bottomWall{sf::Vector2f(static_cast<float>(width), static_cast<float>(thickness))};
        bottomWall.setOrigin(bottomWall.getSize() / 2.0f);
        bottomWall.setPosition(center + sf::Vector2f(0.f, halfH - halfT));
        bottomWall.setRotation(sf::degrees(rotDeg));
        window.draw(bottomWall);

        sf::RectangleShape leftWall{sf::Vector2f(static_cast<float>(thickness), static_cast<float>(height))};
        leftWall.setOrigin(leftWall.getSize() / 2.0f);
        leftWall.setPosition(center + sf::Vector2f(-halfW + halfT, 0.f));
        leftWall.setRotation(sf::degrees(rotDeg));
        window.draw(leftWall);

        sf::RectangleShape rightWall{sf::Vector2f(static_cast<float>(thickness), static_cast<float>(height))};
        rightWall.setOrigin(rightWall.getSize() / 2.0f);
        rightWall.setPosition(center + sf::Vector2f(halfW - halfT, 0.f));
        rightWall.setRotation(sf::degrees(rotDeg));
        window.draw(rightWall);
    }
};
