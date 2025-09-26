#include "HollowRectObstacle.h"
#include <cmath>
#include <algorithm>

HollowRectObstacle::HollowRectObstacle(double x_, double y_, double width_, double height_, double thickness_, double rotation_, double elasticity_, Color color_)
    : Obstacle(x_, y_, elasticity_, color_), width(width_), height(height_), thickness(thickness_), rotation(rotation_) {}

void HollowRectObstacle::draw() const {
    float halfW = static_cast<float>(width / 2.0);
    float halfH = static_cast<float>(height / 2.0);
    float halfT = static_cast<float>(thickness / 2.0f);
    float rotDeg = static_cast<float>(rotation * 180.0 / M_PI);

    // Top wall
    DrawRectanglePro(
        {static_cast<float>(x), static_cast<float>(y - halfH + halfT)},
        {static_cast<float>(width), static_cast<float>(thickness)},
        rotDeg,
        color
    );

    // Bottom wall
    DrawRectanglePro(
        {static_cast<float>(x), static_cast<float>(y + halfH - halfT)},
        {static_cast<float>(width), static_cast<float>(thickness)},
        rotDeg,
        color
    );

    // Left wall
    DrawRectanglePro(
        {static_cast<float>(x - halfW + halfT), static_cast<float>(y)},
        {static_cast<float>(thickness), static_cast<float>(height)},
        rotDeg,
        color
    );

    // Right wall
    DrawRectanglePro(
        {static_cast<float>(x + halfW - halfT), static_cast<float>(y)},
        {static_cast<float>(thickness), static_cast<float>(height)},
        rotDeg,
        color
    );
}

void HollowRectObstacle::collide(Particle& p, double dt) const {
    double cosA = std::cos(rotation);
    double sinA = std::sin(rotation);

    double halfW = width / 2.0;
    double halfH = height / 2.0;
    double halfT = thickness / 2.0;

    auto checkWall = [&](double cx, double cy, double w, double h) {
        double relX = p.x - cx;
        double relY = p.y - cy;

        double localX =  cosA * relX + sinA * relY;
        double localY = -sinA * relX + cosA * relY;

        double clampedX = std::clamp(localX, -w/2.0, w/2.0);
        double clampedY = std::clamp(localY, -h/2.0, h/2.0);

        double closestX = cosA * clampedX - sinA * clampedY + cx;
        double closestY = sinA * clampedX + cosA * clampedY + cy;

        double dx = p.x - closestX;
        double dy = p.y - closestY;
        double dist2 = dx*dx + dy*dy;

        double speed2 = p.vx * p.vx + p.vy * p.vy;
        double threshold = (std::min(thickness, static_cast<double>(p.radius)) / dt);
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
    };

    checkWall(x, y - halfH + halfT, width, thickness);
    checkWall(x, y + halfH - halfT, width, thickness);
    checkWall(x - halfW + halfT, y, thickness, height);
    checkWall(x + halfW - halfT, y, thickness, height);
}
