#pragma once
#include "../include/Interactive.h"
#include "../include/Forcefield.h"

struct MouseEffect : Effect {
    Vector2 center;
    Vector2 velocity;
    double strength;
    double radius;

    MouseEffect(double radius_, double strength_)
        : center({0.f, 0.f}), velocity({0.f, 0.f}), strength(strength_), radius(radius_) {}

    void apply(Particle& p, double dt) const override {
        double dx = p.x - center.x;
        double dy = p.y - center.y;
        double dist = std::sqrt(dx * dx + dy * dy);
        if (dist >= radius || p.mass <= 0) return;

        double falloff = 1.0 - (dist / radius);
        double fx = velocity.x * strength * falloff;
        double fy = velocity.y * strength * falloff;

        p.ax += fx / p.mass;
        p.ay += fy / p.mass;

        p.x += fx * dt / p.mass;
        p.y += fy * dt / p.mass;

        p.px += fx * dt * 0.01 / p.mass;
        p.py += fy * dt * 0.01 / p.mass;
    }

    void reset() {
        velocity = {0.f, 0.f};
    }
};

struct MovableCircleArea : public CircleArea {
    MovableCircleArea(float x_, float y_, float r_, Color color_ = WHITE)
        : CircleArea(x_, y_, r_, color_) {}

    void setPosition(const Vector2& pos) { x = pos.x; y = pos.y; }
    Vector2 getPosition() const { return {x, y}; }
};

struct MouseForcefield : public Interactive, public Forcefield {
    Vector2 prevPos{0.f, 0.f};
    bool firstFrame = true;

    MouseForcefield(double radius, double strength)
        : Forcefield(std::make_unique<MovableCircleArea>(0, 0, radius, BLACK),
                     std::make_unique<MouseEffect>(radius, strength)) {}

    void handleEvent(double dt) override {
        Vector2 currPos = GetMousePosition();

        Vector2 vel{0.f, 0.f};
        if (!firstFrame) {
            vel.x = (currPos.x - prevPos.x) / static_cast<float>(dt);
            vel.y = (currPos.y - prevPos.y) / static_cast<float>(dt);
        } else {
            firstFrame = false;
        }
        prevPos = currPos;

        if (auto* circle = dynamic_cast<MovableCircleArea*>(area.get()))
            circle->setPosition(currPos);

        if (auto* effect = dynamic_cast<MouseEffect*>(this->effect.get())) {
            effect->center = currPos;
            effect->velocity = vel;
        }
    }

    void resetEffectVelocity() {
        if (auto* effect = dynamic_cast<MouseEffect*>(this->effect.get()))
            effect->reset();
    }
};