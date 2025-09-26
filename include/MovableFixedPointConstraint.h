#pragma once
#include "FixedPointConstraint.h"
#include "Interactive.h"

struct MovableFixedPointConstraint : public FixedPointConstraint, public Interactive {
    bool isDragging = false;
    float dragStartX, dragStartY;
    float dragCurrentX, dragCurrentY;

    MovableFixedPointConstraint(Particle* particle, double x, double y)
        : FixedPointConstraint(particle, x, y) {}

    void handleEvent(double dt) override {
        float mouseX = static_cast<float>(GetMouseX());
        float mouseY = static_cast<float>(GetMouseY());

        if (!isDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            float dx = mouseX - fx;
            float dy = mouseY - fy;
            if (dx*dx + dy*dy <= 100.f) {
                isDragging = true;
                dragStartX = mouseX;
                dragStartY = mouseY;
                dragCurrentX = mouseX;
                dragCurrentY = mouseY;
            }
        } 
        else if (isDragging && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            isDragging = false;
        }

        if (isDragging) {
            dragCurrentX = mouseX;
            dragCurrentY = mouseY;
            fx = dragCurrentX;
            fy = dragCurrentY;
        }
    }

    void draw() const override {
        return;
    }
};