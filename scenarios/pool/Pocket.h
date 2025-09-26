#pragma once
#include "../../include/SolidCircleObstacle.h"
#include "Ball.h"

struct Pocket : public SolidCircleObstacle {
    Pocket(double x_, double y_, double radius_)
        : SolidCircleObstacle(x_, y_, radius_, 0.0, BLACK) {}

    void draw() const override {
        DrawCircleV({static_cast<float>(x), static_cast<float>(y)},
                    static_cast<float>(radius),
                    color);
    }

    void collide(Particle& p, double dt) const override {
        double dx = p.x - x;
        double dy = p.y - y;
        double dist2 = dx*dx + dy*dy;
        if (dx*dx + dy*dy <= radius*radius) {
            if (auto cue = dynamic_cast<Ball*>(&p)) {
                cue->alive = false;
            }
        }
    }
};