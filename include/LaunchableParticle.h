#pragma once
#include <cmath>
#include "Particle.h"
#include "Interactive.h"

struct LaunchableParticle : public Particle, public Interactive {
    bool isDragging = false;
    float dragStartX = 0, dragStartY = 0;
    float dragCurrentX = 0, dragCurrentY = 0;
    double maxForce;
    double forceScale;

    LaunchableParticle(double x_, double y_, double mass_ = 1.0, float radius_ = 5.0, double elasticity_ = 1.0, Color color_ = WHITE, double maxForce_ = 500.0f, double forceScale_ = 4.0f)
        : Particle(x_, y_, 0.0, 0.0, mass_, radius_, elasticity_, color_), maxForce(maxForce_), forceScale(forceScale_) {}

    void handleEvent(double dt) override {
        const double EPS = 1e-1;
        if (std::abs(vx) < EPS) vx = 0;
        if (std::abs(vy) < EPS) vy = 0;
        if (vx != 0 || vy != 0) return;

        float mouseX = static_cast<float>(GetMouseX());
        float mouseY = static_cast<float>(GetMouseY());
        bool leftPressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

        if (!isDragging && leftPressed) {
            float dx = mouseX - x;
            float dy = mouseY - y;
            if (dx*dx + dy*dy <= radius*radius) {
                isDragging = true;
                dragStartX = mouseX;
                dragStartY = mouseY;
                dragCurrentX = mouseX;
                dragCurrentY = mouseY;
            }
        }

        if (isDragging && leftPressed) {
            dragCurrentX = mouseX;
            dragCurrentY = mouseY;
        }

        if (isDragging && !leftPressed) {
            float pullX = dragStartX - dragCurrentX;
            float pullY = dragStartY - dragCurrentY;
            float length = std::sqrt(pullX * pullX + pullY * pullY);
            if (length > 0.f) {
                float dirX = pullX / length;
                float dirY = pullY / length;
                float forceMag = length * forceScale;
                if (forceMag > maxForce) forceMag = static_cast<float>(maxForce);

                vx = static_cast<double>(dirX * forceMag / mass);
                vy = static_cast<double>(dirY * forceMag / mass);
            }
            isDragging = false;
        }
    }

    void draw() const override {
        DrawCircle(static_cast<int>(x), static_cast<int>(y), radius, color);

        if (isDragging) {
            float toMouseX = dragCurrentX - x;
            float toMouseY = dragCurrentY - y;
            float dist = std::sqrt(toMouseX * toMouseX + toMouseY * toMouseY);

            if (dist > 0.f) {
                float dirX = toMouseX / dist;
                float dirY = toMouseY / dist;

                float lineEndX = x - dirX * (radius + dist);
                float lineEndY = y - dirY * (radius + dist);

                DrawLine(static_cast<int>(x), static_cast<int>(y),
                         static_cast<int>(lineEndX), static_cast<int>(lineEndY),
                         YELLOW);
            }
        }
    }
};