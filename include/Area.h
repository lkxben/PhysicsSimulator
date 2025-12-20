#pragma once
#include "Particle.h"
#include "raylib.h"
#include <cmath>

struct Area {
    Color color;

    Area(Color color_ = WHITE) : color(color_) {}

    virtual bool contains(const Particle& p) const = 0;
    virtual void draw() const = 0;
    virtual ~Area() = default;
};

struct CircleArea : Area {
    float x, y, radius;

    CircleArea(float x_, float y_, float r_, Color color_ = WHITE)
        : Area(color_), x(x_), y(y_), radius(r_) {}

    bool contains(const Particle& p) const override {
        double dx = static_cast<double>(p.x) - static_cast<double>(x);
        double dy = static_cast<double>(p.y) - static_cast<double>(y);
        return dx * dx + dy * dy <= static_cast<double>(radius) * radius;
    }

    void draw() const override {
        DrawCircleV({ static_cast<float>(x), static_cast<float>(y) }, static_cast<float>(radius),
                    CLITERAL(Color){color.r, color.g, color.b, color.a});
    }
};

struct RectArea : Area {
    float x, y, width, height;

    RectArea(float x_, float y_, float w_, float h_, Color color_ = WHITE)
        : Area(color_), x(x_), y(y_), width(w_), height(h_) {}

    bool contains(const Particle& p) const override {
        double halfW = static_cast<double>(width) / 2.0;
        double halfH = static_cast<double>(height) / 2.0;
        double dx = p.x - static_cast<double>(x);
        double dy = p.y - static_cast<double>(y);
        return dx >= -halfW && dx <= halfW &&
               dy >= -halfH && dy <= halfH;
    }

    void draw() const override {
        Rectangle rect{};
        rect.x = x - width / 2.0f;
        rect.y = y - height / 2.0f;
        rect.width = width;
        rect.height = height;

        DrawRectangleRec(rect, CLITERAL(Color){color.r, color.g, color.b, color.a});
    }
};