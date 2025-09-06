#include "SolidRectObstacle.h"
#include <cmath>
#include <algorithm>

SolidRectObstacle::SolidRectObstacle(double x_, double y_, double width_, double height_, double rotation_, double elasticity_, sf::Color color_)
    : Obstacle(x_, y_, elasticity_, color_), width(width_), height(height_), rotation(rotation_) {}

void SolidRectObstacle::draw(sf::RenderWindow& window) const {
    sf::RectangleShape shape{sf::Vector2f(static_cast<float>(width), static_cast<float>(height))};
    shape.setOrigin({static_cast<float>(width)/2, static_cast<float>(height)/2});
    shape.setPosition({static_cast<float>(x), static_cast<float>(y)});
    shape.setRotation(sf::degrees(static_cast<float>(rotation * 180.0 / M_PI)));
    shape.setFillColor(color);
    window.draw(shape);
}

void SolidRectObstacle::collide(Particle& p, double dt) const {
    double cosA = std::cos(rotation);
    double sinA = std::sin(rotation);

    double relX = p.x - x;
    double relY = p.y - y;

    double localX =  cosA * relX + sinA * relY;
    double localY = -sinA * relX + cosA * relY;

    double halfW = width / 2.0;
    double halfH = height / 2.0;
    double clampedX = std::clamp(localX, -halfW, halfW);
    double clampedY = std::clamp(localY, -halfH, halfH);

    double closestX = cosA * clampedX - sinA * clampedY + x;
    double closestY = sinA * clampedX + cosA * clampedY + y;

    double dx = p.x - closestX;
    double dy = p.y - closestY;
    double dist2 = dx*dx + dy*dy;

    double speed2 = p.vx * p.vx + p.vy * p.vy;
    double threshold = p.radius / dt;
    double threshold2 = threshold * threshold;

    if (speed2 > threshold2) {
        double prevX = p.x - p.vx * dt;
        double prevY = p.y - p.vy * dt;

        double segX = closestX - prevX;
        double segY = closestY - prevY;
        double segLen2 = segX*segX + segY*segY;
        if (segLen2 > 0) {
            double t = ((p.vx)*segX + (p.vy)*segY) / segLen2;
            t = std::clamp(t, 0.0, 1.0);
            double projX = prevX + t * p.vx * dt;
            double projY = prevY + t * p.vy * dt;
            double cdx = projX - closestX;
            double cdy = projY - closestY;
            if (cdx*cdx + cdy*cdy < p.radius * p.radius) {
                dx = cdx;
                dy = cdy;
                dist2 = dx*dx + dy*dy;
            }
        }
    }

    if (dist2 < p.radius * p.radius) {
        double dist = std::sqrt(dist2);
        if (dist == 0.0) { dx = 1e-8; dy = 0; dist = 1e-8; }

        double nx = dx / dist;
        double ny = dy / dist;

        double overlap = p.radius - dist;
        p.x += nx * overlap;
        p.y += ny * overlap;

        double vDotN = p.vx * nx + p.vy * ny;
        if (vDotN < 0) {
            double e = 0.5 * (p.elasticity + elasticity);
            p.vx -= (1 + e) * vDotN * nx;
            p.vy -= (1 + e) * vDotN * ny;
        }
    }
}
